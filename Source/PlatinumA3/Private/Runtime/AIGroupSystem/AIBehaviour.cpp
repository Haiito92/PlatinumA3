// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviour.h"

void UAIBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
}

bool UAIBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	return false;
}

void UAIBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
}

void UAIBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
}

void UAIBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
}
