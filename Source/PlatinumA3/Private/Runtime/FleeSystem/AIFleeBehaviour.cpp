// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

void UAIFleeBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	
	UFleeSubsystem * tFleeSubsystem = GetWorld()->GetSubsystem<UFleeSubsystem>();
	if(tFleeSubsystem != nullptr)
	{
		FleeSubsystem = tFleeSubsystem;
		// GEngine->AddOnScreenDebugMessage(
		// -1,
		// 3.0f,
		// FColor::Purple,
		// TEXT("Get FLEE Subsystem"));
	}
	
	FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return;
	const FName& Tag = FleeSystemSettings->TagToFleeFrom;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	ActorsToFleeFrom = FoundActors;

	FleeLeaderComponents.Init(nullptr, Pawns.Num());

	for (int i = 0; i < Pawns.Num(); ++i)
	{
		UFleeLeaderComponent* FleeLeaderComponent = Pawns[i]->FindComponentByClass<UFleeLeaderComponent>();
		if(FleeLeaderComponent == nullptr) continue;

		FleeLeaderComponents[i] = FleeLeaderComponent;
	}
	
}

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	bool valid = false;

	// const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	// if(FleeSystemSettings == nullptr) return false;
	//
	// for (AActor* Actor : ActorsToFleeFrom)
	// {
	// 	if(FVector::Distance(Pawn->GetActorLocation(), Actor->GetActorLocation()) < FleeSystemSettings->FleeDetectionRadius)
	// 	{
	// 		valid = true;
	// 	}
	// }
	
	return FleeLeaderComponents[Pawn->GetIndex()]->GetFleeing();
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);
	
	// UFleeLeaderComponent* LeaderComponent = FleeLeaderComponents[Pawn->GetIndex()];
	// if(FleeSubsystem == nullptr || LeaderComponent == nullptr) return;
	// FleeSubsystem->GetCurrentFleeLeaders().Add(Pawn->GetIndex(),LeaderComponent);
	
	if(FleeSystemSettings == nullptr) return;
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->bCanWalkOffLedges = FleeSystemSettings->CanFleeWalkOffLedges;
		MovementComponent->MaxWalkSpeed = FleeSystemSettings->FleeSpeed;
	}
	
	Pawn->StartMovingAICharacter();
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("FLEE ENTRY"));
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	int Index = Pawn->GetIndex();

	FVector Direction = FVector::Zero();

	const FVector PawnLocation = Pawn->GetActorLocation();

	if(FleeSystemSettings == nullptr) return;
	
	for (const AActor* ActorToFleeFrom : FleeLeaderComponents[Index]->GetScaryActors())
	{
		FVector AtoP = PawnLocation - ActorToFleeFrom->GetActorLocation();
		Direction += AtoP;
	}

	Direction.Z = 0;
	Direction.Normalize();

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, PawnLocation+Direction);
	
	Pawn->StartRotateAICharacter(LookAtRotation);
	
	Pawn->AddMovementInput(Direction,1.0f);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	4.0f,
	// 	FColor::Orange,
	// 	TEXT("FLEE"));
}
void UAIFleeBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	Pawn->StopRotateAICharacter();
	Pawn->StopMovingAICharacter();
	// if(FleeSubsystem == nullptr) return;
	// FleeSubsystem->GetCurrentFleeLeaders().Remove(Pawn->GetIndex());
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("FLEE EXIT"));
}
