// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIBehaviourSettings.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"

void UAIFleeBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
{
	Super::InitBehaviour(Pawns);


	const UAIBehaviourSettings* BehaviourSettings = GetDefault<UAIBehaviourSettings>();
	if(BehaviourSettings == nullptr) return;
	const FName& Tag = BehaviourSettings->TagToFleeFrom;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	ActorsToFleeFrom = FoundActors;
}

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	bool valid = false;

	for (AActor* Actor : ActorsToFleeFrom)
	{
		if(FVector::Distance(Pawn->GetActorLocation(), Actor->GetActorLocation()) < 300.0f)
		{
			valid = true;
		}
	}
	
	return valid;
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
	Super::BehaviourEntry(Pawn);

	UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = 600.0f;
	}

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FLEE ENTRY"));
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	FVector direction = FVector::Zero();

	for (AActor* ActorToFleeFrom : ActorsToFleeFrom)
	{
		FVector AtoP = Pawn->GetActorLocation() - ActorToFleeFrom->GetActorLocation();
		if(AtoP.Length() < 300.0f)
		{
			direction += AtoP;
		}
	}

	direction.Normalize();
	direction.Z = 0;

	Pawn->AddMovementInput(direction);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	4.0f,
	// 	FColor::Orange,
	// 	TEXT("FLEE"));
}

void UAIFleeBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
{
	Super::BehaviourExit(Pawn);

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FLEE EXIT"));
}
