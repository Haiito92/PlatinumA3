// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIIdleBehaviour.h"

bool UAIIdleBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	return true;
}

void UAIIdleBehaviour::BehaviourEntry(AAIGroupPawn* Pawn) const
{
	Super::BehaviourEntry(Pawn);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("IDLE ENTRY"));
}

void UAIIdleBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn) const
{
	Super::BehaviourUpdate(Pawn);

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("IDLE"));
}

void UAIIdleBehaviour::BehaviourExit(AAIGroupPawn* Pawn) const
{
	Super::BehaviourExit(Pawn);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("IDLE EXIT"));
}
