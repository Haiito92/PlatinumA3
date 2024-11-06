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
	virtual void InitBehaviour();
	
	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn) const;
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn) const;
	virtual void BehaviourExit(AAIGroupPawn* Pawn) const;
};
