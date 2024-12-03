// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIFollowFleeBehaviour.generated.h"

class UFleeSystemSettings;
class UFleeFollowerComponent;
class UFleeSubsystem;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIFollowFleeBehaviour : public UAIBehaviour
{
	GENERATED_BODY()

#pragma region Behaviour Defaults
public:
	virtual void InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns) override;

	virtual bool CheckBehaviourValidity(AAIGroupCharacter* Pawn) override;

	virtual void BehaviourEntry(AAIGroupCharacter* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupCharacter* Pawn) override;
#pragma endregion

#pragma region FollowFlee
	UPROPERTY()
	TObjectPtr<UFleeSubsystem> FleeSubsystem;

	UPROPERTY()
	const UFleeSystemSettings* FleeSystemSettings;

	UPROPERTY()
	TArray<UFleeFollowerComponent*> FleeFollowerComponents;
#pragma endregion
};
