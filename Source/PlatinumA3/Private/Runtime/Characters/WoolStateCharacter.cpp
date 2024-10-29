// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Characters/WoolStateCharacter.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Berger/Catchable.h"
#include "Runtime/Chien/Biteable.h"


// Sets default values
AWoolStateCharacter::AWoolStateCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Initialize Physics Handle Component
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	HoldingTarget = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingTarget"));
	if (GetMesh())
	{
		HoldingTarget->SetupAttachment(GetMesh());
	}
}

// Called when the game starts or when spawned
void AWoolStateCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWoolStateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsHoldingSomething)
	{
		
	}
}

// Called to bind functionality to input
void AWoolStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWoolStateCharacter::StartHolding()
{
	AActor* Catchable = GetSomethingToHold();

	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;
	
	if(IsValid(Catchable))
	{
		// Check if the actor implements the interface
		if (Catchable->Implements<UCatchable>())
		{
			PrimitiveComponent = ICatchable::Execute_Catch(Catchable);
		}
	}

	if(PrimitiveComponent)
	{
		Original_SimulatePhysics = PrimitiveComponent->IsSimulatingPhysics();
		Original_CollisionProfileName = PrimitiveComponent->GetCollisionProfileName();

		PrimitiveComponent->SetSimulatePhysics(true);
		PrimitiveComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

		PrimitiveComponent->SetLinearDamping(1.2f);
		PrimitiveComponent->SetAngularDamping(4.0f);

		PhysicsHandle->GrabComponentAtLocationWithRotation(PrimitiveComponent, FName(), HoldingTarget->GetComponentLocation(), HoldingTarget->GetComponentRotation());
		IsHoldingSomething = true;
	}
}

void AWoolStateCharacter::StopHolding(float TransTime)
{
	if(IsHoldingSomething)
	{
		TObjectPtr<UPrimitiveComponent> HeldComponent = PhysicsHandle->GrabbedComponent;

		AActor* Catchable = HeldComponent->GetOwner();
		if (Catchable && Catchable->Implements<UCatchable>())
		{
			ICatchable::Execute_Launch(Catchable, Original_SimulatePhysics, Original_CollisionProfileName, TransTime);
		}
		
		PhysicsHandle->ReleaseComponent();
		IsHoldingSomething = false;
	}
}

void AWoolStateCharacter::UpdateHolding()
{
	if(IsHoldingSomething)
	{
		FVector Location = HoldingTarget->GetComponentLocation();
		Location += FVector(0, 0, 100);
		FRotator Rotation = HoldingTarget->GetComponentRotation();
		
		PhysicsHandle->SetTargetLocationAndRotation(Location, Rotation);
	}
}

void AWoolStateCharacter::LaunchSomething()
{
	UPrimitiveComponent* HeldComponent = PhysicsHandle->GrabbedComponent;

	if(HeldComponent)
	{
		StopHolding(LaunchTransTime);
		
		// Apply impulse
		FVector ThrowDirection = GetActorForwardVector();
		HeldComponent->AddImpulse(ThrowDirection * ThrowStrength, NAME_None, true);

		// Clear HeldComponent reference if needed
		HeldComponent = nullptr;
	}
	
}


AActor* AWoolStateCharacter::GetSomethingToHold()
{
	
	FVector Center = GetActorLocation();
	float Radius = 250.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	
	if (bOverlap)
	{
		DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Red, false, 1.0f);

		for (auto& Result : OverlapResults)
		{
			if (AActor* OverlappedActor = Result.GetActor())
			{
				
				if (OverlappedActor->Implements<UCatchable>())
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Actor with catchable interface: %s"), *OverlappedActor->GetName());
					return OverlappedActor;
				}
			}
		}
	}

	return nullptr;

}








void AWoolStateCharacter::LaunchBite()
{
	AActor* ActorToBite = GetSomethingToBite();

	if(IsValid(ActorToBite))
	{
		// Check if the actor implements the interface
		if (ActorToBite->Implements<UBiteable>())
		{
			IBiteable::Execute_Bite(ActorToBite);
		}
	}
}




AActor* AWoolStateCharacter::GetSomethingToBite()
{
	
	FVector Center = GetActorLocation();
	float Radius = 250.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	
	if (bOverlap)
	{
		DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Purple, false, 1.0f);

		for (auto& Result : OverlapResults)
		{
			if (AActor* OverlappedActor = Result.GetActor())
			{
				
				if (OverlappedActor->Implements<UBiteable>())
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Actor with biteable interface: %s"), *OverlappedActor->GetName());
					return OverlappedActor;
				}
			}
		}
	}

	return nullptr;

}