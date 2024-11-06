// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIIdleBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIIdleBehaviour : public UAIBehaviour
{
	GENERATED_BODY()

public:
	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn) const override;
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn) const override;
	virtual void BehaviourExit(AAIGroupPawn* Pawn) const override;
};
