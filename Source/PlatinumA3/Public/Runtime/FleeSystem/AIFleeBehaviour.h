// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "UObject/Object.h"
#include "AIFleeBehaviour.generated.h"

/**
 * 
 */

class UFleeSubsystem;
class UFleeLeaderComponent;

UCLASS()
class PLATINUMA3_API UAIFleeBehaviour : public UAIBehaviour
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<UFleeSubsystem> FleeSubsystem;
	
	UPROPERTY()
	TArray<AActor*> ActorsToFleeFrom;

	UPROPERTY()
	TArray<UFleeLeaderComponent*> FleeLeaderComponents;
	
public:
	virtual void InitBehaviour(const TArray<AAIGroupPawn*>& Pawns) override;
	
	virtual bool CheckBehaviourValidity(AAIGroupPawn* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupPawn* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupPawn* Pawn) override;
};
