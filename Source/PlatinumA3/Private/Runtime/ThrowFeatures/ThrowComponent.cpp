// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/ThrowFeatures/ThrowComponent.h"

#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Berger/Catchable.h"


// Sets default values for this component's properties
UThrowComponent::UThrowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UThrowComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Catchable = GetOwner();

	if(ACharacter* Character = Cast<ACharacter>(Catchable))
	{
		Character->LandedDelegate.AddDynamic(this, &UThrowComponent::StartExecuteLaunch);
	}
}


// Called every frame
void UThrowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHolding();
	// ...
}

void UThrowComponent::Catch(UPhysicsHandleComponent* InPhysicsHandle, USceneComponent* InHoldingTarget)
{
	HoldingTarget = InHoldingTarget;
	PhysicsHandle = InPhysicsHandle;
	
	AActor* Catchable = GetOwner();

	if(Catchable)
	{
		if (Catchable->Implements<UCatchable>())
		{
			PrimitiveComponent = ICatchable::Execute_Catch(Catchable);
		}
	}
	
	if(PrimitiveComponent && PhysicsHandle && HoldingTarget)
	{
		Original_SimulatePhysics = PrimitiveComponent->IsSimulatingPhysics();
		Original_CollisionProfileName = PrimitiveComponent->GetCollisionProfileName();
	
		PrimitiveComponent->SetSimulatePhysics(true);
		//PrimitiveComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	
		PrimitiveComponent->SetLinearDamping(1.2f);
		PrimitiveComponent->SetAngularDamping(4.0f);
	
		PhysicsHandle->GrabComponentAtLocationWithRotation(PrimitiveComponent, FName(), HoldingTarget->GetComponentLocation(), HoldingTarget->GetComponentRotation());
		IsHoldingSomething = true;
	}
}

void UThrowComponent::UpdateHolding()
{
	if(IsHoldingSomething && PhysicsHandle)
	{
		FVector Location = HoldingTarget->GetComponentLocation();
		Location += FVector(0, 0, 100);
		FRotator Rotation = HoldingTarget->GetComponentRotation();
		
		PhysicsHandle->SetTargetLocationAndRotation(Location, Rotation);
	}
}

void UThrowComponent::StopHolding()
{
	if(PrimitiveComponent)
	{
		PrimitiveComponent->SetSimulatePhysics(false);
	}
}

void UThrowComponent::Launch(bool IsSimulatingPhysic, FName CollisionProfilName, float TransTime)
{
	
}

void UThrowComponent::StartExecuteLaunch(const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Purple,
		TEXT("ON LANDED")
	);
}

