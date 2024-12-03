// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviour.h"

#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

void UAIBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	// PawnBehaviourDatas.Init({FRotator::ZeroRotator}, Pawns.Num());
}

bool UAIBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	return false;
}

void UAIBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	// Pawn->SetBehaviourStartingRotation(Pawn->GetActorRotation());
	
	// FAIPawnBehaviourData& Data = PawnBehaviourDatas[Pawn->GetIndex()];
	// Data.StartingRotation = Pawn->GetActorRotation();
}

void UAIBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
}

void UAIBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
}
