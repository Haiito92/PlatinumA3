// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AIBehaviour.generated.h"

/**
 * 
 */

class AAIGroupPawn;

UCLASS()
class PLATINUMA3_API UAIBehaviour : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void InitBehaviour(const TArray<AAIGroupPawn*>& Pawns);
	
	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn);
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime);
	virtual void BehaviourExit(AAIGroupPawn* Pawn);
};
