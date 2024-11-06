// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "UObject/Object.h"
#include "AIFleeBehaviour.generated.h"

/**
 * 
 */

UCLASS()
class PLATINUMA3_API UAIFleeBehaviour : public UAIBehaviour
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> ActorToFleeFrom;
	
public:
	virtual void InitBehaviour() override;
	
	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn) const override;
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn) const override;
	virtual void BehaviourExit(AAIGroupPawn* Pawn) const override;
};
