// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviour.h"

void UAIBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
{
}

bool UAIBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	return false;
}

void UAIBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
}

void UAIBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
}

void UAIBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
{
}
