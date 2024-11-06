// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	return false;
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupPawn* Pawn) const
{
	Super::BehaviourEntry(Pawn);
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn) const
{
	Super::BehaviourUpdate(Pawn);

	Pawn->AddMovementInput(FVector::ForwardVector, 100);
	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Orange,
		TEXT("FLEE"));
}

void UAIFleeBehaviour::BehaviourExit(AAIGroupPawn* Pawn) const
{
	Super::BehaviourExit(Pawn);
}
