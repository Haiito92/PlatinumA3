// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIIdleBehaviour.generated.h"

USTRUCT()
struct FIdlePawnData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float Timer;
	// UPROPERTY()
	// FTimerHandle TimerHandle;
	UPROPERTY()
	FVector IdlingDirection;
};
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIIdleBehaviour : public UAIBehaviour
{
	GENERATED_BODY()
#pragma region Behaviour Defaults
	
public:
	virtual void InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns) override;
	
	virtual bool CheckBehaviourValidity(AAIGroupCharacter* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupCharacter* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupCharacter* Pawn) override;
#pragma endregion 
	
private:
	TArray<FIdlePawnData> IdlingPawnDatas;

	void GivePawnNewDirection(FIdlePawnData& Data);
};
