// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/ThrowFeatures/ThrowComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void UThrowComponent::OnCustomLandedFunc()
{
	GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Purple,
			TEXT("ON LANDED DELEGATE")
			);

	PrimitiveComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	PrimitiveComponent->SetSimulatePhysics(false);
	PrimitiveComponent->SetRelativeRotation(FRotator::ZeroRotator);
	
	Landing_Delegate.Broadcast();

	LandingStep_Delegate.RemoveDynamic(this, &UThrowComponent::OnCustomLandedFunc);
}

void UThrowComponent::OnPhysicsHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Log, TEXT("Character landed on: %s"), *OtherActor->GetName());

		// Additional logic for "landing" detection
		FVector HitNormal = Hit.Normal;
		if (HitNormal.Z > 0.7f) // Check if the hit surface is mostly horizontal
		{
			UE_LOG(LogTemp, Log, TEXT("Landed on ground-like surface!"));
		}
	}
}

void UThrowComponent::HandleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Purple,
		TEXT("ON COMPONENT HIT SOMETHING")
		);


	if(PrimitiveComponent)
	{
		PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
		PrimitiveComponent->SetSimulatePhysics(false);
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
	Catch_Delegate.Broadcast();
	
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

		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		PrimitiveComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		
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
		//FRotator Rotation = HoldingTarget->GetComponentRotation();

		PhysicsHandle->SetTargetLocationAndRotation(Location, FRotator::ZeroRotator);
	}
}

void UThrowComponent::StopHolding()
{
	if(PrimitiveComponent && PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void UThrowComponent::CheckForLanding()
{
	PrimitiveComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	float DistanceOffset = 0;
	if(ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		DistanceOffset = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}


	
	// Get the character's location
	FVector CharacterLocation = GetOwner()->GetActorLocation();

	// Define the start and end points of the line trace
	FVector Start = CharacterLocation;
	FVector End = CharacterLocation - FVector(0.f, 0.f, 200.f);

	// Trace parameters
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); 

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		Start, 
		End, 
		ECC_Visibility,
		QueryParams
	);

	float GroundDistance;
	// Process the result
	if (bHit)
	{
		GroundDistance = (Start - HitResult.Location).Size();
		GroundDistance -= DistanceOffset;
		
		UE_LOG(LogTemp, Warning, TEXT("Distance to ground: %f"), GroundDistance);

		if(GroundDistance < 1.0f)
		{
			LandingStep_Delegate.Broadcast();
		}
		// Debug line (optional)
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 1.f, 0, 2.f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.f, FColor::Red, false, 1.f);
	}
	else
	{
		GroundDistance = -1.f;
		UE_LOG(LogTemp, Warning, TEXT("No ground detected!"));

		// Debug line (optional)
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 2.f);
	}
}

void UThrowComponent::Launch()
{

	if(PrimitiveComponent)
	{
		PhysicsHandle->ReleaseComponent();

		// Apply impulse
		FVector ThrowDirection = PhysicsHandle->GetOwner()->GetActorForwardVector();
		PrimitiveComponent->AddImpulse(ThrowDirection * ThrowStrength, NAME_None, true);


		// TEST
		PrimitiveComponent->BodyInstance.bLockXRotation = true;
		PrimitiveComponent->BodyInstance.bLockYRotation = true;
		PrimitiveComponent->BodyInstance.bLockZRotation = true;
		
		GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,                   
		this,                          
		&UThrowComponent::CheckForLanding,
		0.02f,                          
		true                           
		);

		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			LandingStep_Delegate.AddDynamic(this, &UThrowComponent::OnCustomLandedFunc);
		}
		
		// Clear HeldComponent reference if needed
		//PrimitiveComponent = nullptr;
	}

	AActor* Catchable = GetOwner();
	if(ACharacter* Character = Cast<ACharacter>(Catchable))
	{
		// Bind to the OnComponentHit event
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}

	Throw_Delegate.Broadcast();
	StopHolding();
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

