// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/AIFollowFleeBehaviour.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

#pragma region Unreal Defaults
void UAIFollowFleeBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	UFleeSubsystem * tFleeSubsystem = GetWorld()->GetSubsystem<UFleeSubsystem>();
	if(tFleeSubsystem != nullptr)
	{
		FleeSubsystem = tFleeSubsystem;
	}
}

bool UAIFollowFleeBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	bool Valid = false;
	for (TTuple<int, UFleeLeaderComponent*> Pair : FleeSubsystem->GetCurrentFleeLeaders())
	{
		if(Pair.Key == Pawn->GetIndex()) continue;
		
		FVector LeaderLocation = Pair.Value->GetOwner()->GetActorLocation();
		
		if(FVector::Distance(LeaderLocation, Pawn->GetActorLocation()) < 400.0f)
		{
			Valid = true;
			break;
		}
	}
	
	return Valid;
}

void UAIFollowFleeBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
	Super::BehaviourEntry(Pawn);

	const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return;
	
	UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = FleeSystemSettings->FleeSpeed;
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FOLLOW FLEE ENTRY"));
}

void UAIFollowFleeBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	FVector Direction;
	
	for (TTuple<int, UFleeLeaderComponent*> Pair : FleeSubsystem->GetCurrentFleeLeaders())
	{
		if(Pair.Key == Pawn->GetIndex()) continue;
		const UFleeLeaderComponent* FleeLeaderComponent = Pair.Value;
		FVector LeaderLocation = FleeLeaderComponent->GetOwner()->GetActorLocation();
		if(FVector::Distance(LeaderLocation, Pawn->GetActorLocation()) < 400.0f)
		{
			Direction += FleeLeaderComponent->GetOwner()->GetActorForwardVector();
		}
	}

	Direction.Z = 0;
	Direction.Normalize();

	const FVector PawnLocation = Pawn->GetActorLocation();
	
	Pawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PawnLocation, PawnLocation+Direction));
	Pawn->AddMovementInput(Direction);
}

void UAIFollowFleeBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
{
	Super::BehaviourExit(Pawn);

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FOLLOW FLEE EXIT"));
}
#pragma endregion 