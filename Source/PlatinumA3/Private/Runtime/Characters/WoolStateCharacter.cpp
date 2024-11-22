// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Characters/WoolStateCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Berger/Catchable.h"
#include "Runtime/Berger/Rallyable.h"
#include "Runtime/Characters/InteractInterface.h"
#include "Runtime/Chien/Biteable.h"
#include "Runtime/ThrowFeatures/ThrowComponent.h"


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
}

// Called to bind functionality to input
void AWoolStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWoolStateCharacter::OnLanded(const FHitResult& Hit)
{
	Super::OnLanded(Hit);

	JUICY_OnLanded();
}

void AWoolStateCharacter::StartHolding()
{
	AActor* Catchable = GetSomethingToHold();

	if(!Catchable) return;
	
	ThrowComponent = Catchable->GetComponentByClass<UThrowComponent>();
	
	if(IsValid(ThrowComponent))
	{
		// Check if the actor implements the interface
		ThrowComponent->Catch(PhysicsHandle, HoldingTarget);
		IsHoldingSomething = true;

		JUICY_OnStartHolding();
	}
}

void AWoolStateCharacter::StopHolding(float TransTime)
{
	if(IsHoldingSomething)
	{
		ThrowComponent->StopHolding();
		//PhysicsHandle->ReleaseComponent();
		IsHoldingSomething = false;
		JUICY_OnStopHolding();
	}
}



void AWoolStateCharacter::StartExecuteLaunch(const FHitResult& Hit)
{
	//ICatchable::Execute_Launch(Hit.), Original_SimulatePhysics, Original_CollisionProfileName, LaunchTransTime);
}

void AWoolStateCharacter::LaunchSomething()
{
	if(!ThrowComponent) return;
	ThrowComponent->Launch();
	JUICY_OnThrowSomething();
}


AActor* AWoolStateCharacter::GetSomethingToHold()
{
	
	FVector Center = GetActorLocation();
	float Radius = 150.0f;
	
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
			IBiteable::Execute_Bite(ActorToBite,1.f);
			JUICY_OnBite();
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
				if(Result.Component->GetCollisionProfileName() == "Trigger") continue;
				
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

void AWoolStateCharacter::LaunchRally()
{
	TArray<AActor*> ActorsToRally = GetSomethingToRally();

	for (auto& ActorToRally : ActorsToRally)
	{
		if (ActorToRally->Implements<URallyable>())
		{
			IRallyable::Execute_Rally(ActorToRally, GetActorLocation());
			JUICY_OnRally();
		}
	}
	
}

TArray<AActor*> AWoolStateCharacter::GetSomethingToRally()
{
	FVector Center = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(RallyRadius),
		QueryParams
	);

	TArray<AActor*> ActorsToRally;
	
	if (bOverlap)
	{
		//DrawDebugSphere(GetWorld(), Center, RallyRadius, 12, FColor::Red, false, 1.0f);

		for (auto& Result : OverlapResults)
		{	
			if (AActor* OverlappedActor = Result.GetActor())
			{
				if(Result.Component->GetCollisionProfileName() == "Trigger") continue;
				
				if (OverlappedActor->Implements<URallyable>())
				{
					ActorsToRally.Add(OverlappedActor);
				}
			}
		}
	}

	return ActorsToRally;
}







void AWoolStateCharacter::LaunchInteracting()
{
	AActor* ActorToInteract = GetSomethingToInteractWith();

	// for (auto& ActorToRally : ActorsToRally)
	// {
	// 	if (ActorToRally->Implements<UInteractInterface>())
	// 	{
	// 		IInteractInterface::Execute_Interact(ActorToRally, Cast<APlayerController>(GetOwner()));
	// 	}
	// }


	if(ActorToInteract)
	{

		if(PhysicsHandle->GrabbedComponent)
		{
			StopHolding(LaunchTransTime);
		}
		
		if (ActorToInteract->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_Interact(ActorToInteract, Cast<APlayerController>(GetOwner()));
			JUICY_OnInteract();
		}
	}
	
	
	
}

AActor* AWoolStateCharacter::GetSomethingToInteractWith()
{
	FVector Center = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(InteractRadius),
		QueryParams
	);

	
	if (bOverlap)
	{
		//DrawDebugSphere(GetWorld(), Center, RallyRadius, 12, FColor::Red, false, 1.0f);

		for (auto& Result : OverlapResults)
		{	
			if (AActor* OverlappedActor = Result.GetActor())
			{
				if(Result.Component->GetCollisionProfileName() == "Trigger") continue;
				
				if (OverlappedActor->Implements<UInteractInterface>())
				{
					return OverlappedActor;
				}
			}
		}
	}

	return nullptr;
}