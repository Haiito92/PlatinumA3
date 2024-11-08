// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIRallyBehaviour.generated.h"

class URallyReceiverComponent;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIRallyBehaviour : public UAIBehaviour
{
	GENERATED_BODY()
#pragma region Behaviour Defaults
	virtual void InitBehaviour(const TArray<AAIGroupPawn*>& Pawns) override;

	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupPawn* Pawn) override;
#pragma endregion

private:
	UPROPERTY()
	TArray<URallyReceiverComponent*> RallyReceiverComponents;
};
