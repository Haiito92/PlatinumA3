// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AIBehaviour.generated.h"

/**
 * 
 */
//
// USTRUCT()
// struct FAIPawnBehaviourData
// {
// 	GENERATED_BODY()
//
// public:
// 	UPROPERTY()
// 	FRotator StartingRotation = FRotator::ZeroRotator;
// };

class AAIGroupPawn;
class AAIGroupCharacter;

UCLASS()
class PLATINUMA3_API UAIBehaviour : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns);
	
	virtual bool CheckBehaviourValidity(AAIGroupCharacter* Pawn) const;

	virtual void BehaviourEntry(AAIGroupCharacter* Pawn);
	virtual void BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime);
	virtual void BehaviourExit(AAIGroupCharacter* Pawn);

protected:
	// UPROPERTY()
	// TArray<FAIPawnBehaviourData> PawnBehaviourDatas;
};
