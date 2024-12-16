// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/AIFollowFleeBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeFollowerComponent.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

#pragma region Behaviour Defaults
void UAIFollowFleeBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	UFleeSubsystem * tFleeSubsystem = GetWorld()->GetSubsystem<UFleeSubsystem>();
	if(tFleeSubsystem != nullptr)
	{
		FleeSubsystem = tFleeSubsystem;
	}

	FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	
	FleeFollowerComponents.Init(nullptr, Pawns.Num());
	for (int i = 0; i < Pawns.Num(); ++i)
	{
		UFleeFollowerComponent* FleeFollowerComponent = Pawns[i]->FindComponentByClass<UFleeFollowerComponent>();
		if(FleeFollowerComponent == nullptr) continue;

		FleeFollowerComponents[i] = FleeFollowerComponent;
	}
}

bool UAIFollowFleeBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	bool Valid = false;

	// const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	// if(FleeSystemSettings == nullptr) return false;
	//
	// for (TTuple<int, UFleeLeaderComponent*> Pair : FleeSubsystem->GetCurrentFleeLeaders())
	// {
	// 	if(Pair.Key == Pawn->GetIndex()) continue;
	// 	
	// 	FVector LeaderLocation = Pair.Value->GetOwner()->GetActorLocation();
	// 	
	// 	if(FVector::Distance(LeaderLocation, Pawn->GetActorLocation()) < FleeSystemSettings->FollowFleeDetectionRadius)
	// 	{
	// 		Valid = true;
	// 		break;
	// 	}
	// }
	
	return FleeFollowerComponents[Pawn->GetIndex()]->GetFollowFleeing();
}

void UAIFollowFleeBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);

	if(FleeSystemSettings == nullptr) return;
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->bCanWalkOffLedges = FleeSystemSettings->CanFollowFleeWalkOffLedges;
		MovementComponent->MaxWalkSpeed = FleeSystemSettings->FollowFleeSpeed;
	}

	Pawn->StartMovingAICharacter();
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FOLLOW FLEE ENTRY"));
}

void UAIFollowFleeBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	if(FleeSystemSettings == nullptr) return;
	// for (TTuple<int, UFleeLeaderComponent*> Pair : FleeSubsystem->GetCurrentFleeLeaders())
	// {
	// 	if(Pair.Key == Pawn->GetIndex()) continue;
	// 	const UFleeLeaderComponent* FleeLeaderComponent = Pair.Value;
	// 	FVector LeaderLocation = FleeLeaderComponent->GetOwner()->GetActorLocation();
	// 	if(FVector::Distance(LeaderLocation, Pawn->GetActorLocation()) < FleeSystemSettings->FollowFleeDetectionRadius)
	// 	{
	// 		Direction += FleeLeaderComponent->GetOwner()->GetActorForwardVector();
	// 		Direction.Normalize();
	// 	}
	// }

	UFleeFollowerComponent* FollowerComponent = FleeFollowerComponents[Pawn->GetIndex()];
	if(FollowerComponent == nullptr) return;
	
	FVector Direction;

	for (const TTuple<int, FGroupFollowedData>& Pair : FollowerComponent->GetGroupFollowedDatas())
	{
		Direction += Pair.Value.GroupDirection;
	}
	
	Direction.Z = 0;
	Direction.Normalize();
	

	const FVector PawnLocation = Pawn->GetActorLocation();

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, PawnLocation+Direction);
	
	Pawn->StartRotateAICharacter(LookAtRotation);
	Pawn->AddMovementInput(Direction,1.0f);
}

void UAIFollowFleeBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	Pawn->StopRotateAICharacter();
	Pawn->StopMovingAICharacter();
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FOLLOW FLEE EXIT"));
}
#pragma endregion 