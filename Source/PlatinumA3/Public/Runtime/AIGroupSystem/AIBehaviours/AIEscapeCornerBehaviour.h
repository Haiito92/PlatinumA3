// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIEscapeCornerBehaviour.generated.h"

class UAIDefaultBehavioursSettings;
/**
 * 
 */
UENUM()
enum class EEscapeState: uint8
{
	None,
	NotEscaping,
	Escaping,
	PostEscaping,
	FinishedEscaping
};

USTRUCT()
struct FEscapeCornerPawnData
{
	GENERATED_BODY()
public:
	~FEscapeCornerPawnData() = default;

	UPROPERTY()
	FVector EscapeDirection = FVector::ZeroVector;

	UPROPERTY()
	EEscapeState EscapeState = EEscapeState::NotEscaping;

	UPROPERTY()
	float Timer = 0.0f;
};


UCLASS()
class PLATINUMA3_API UAIEscapeCornerBehaviour : public UAIBehaviour
{
	GENERATED_BODY()
#pragma region BehaviourDefaults
	virtual void InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns) override;
	
	virtual bool CheckBehaviourValidity(AAIGroupCharacter* Pawn) override;

	virtual void BehaviourEntry(AAIGroupCharacter* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupCharacter* Pawn) override;
	
#pragma endregion

private:
	UPROPERTY()
	const UAIDefaultBehavioursSettings* Settings;
	
	UPROPERTY()
	TArray<FEscapeCornerPawnData> EscapeDatas;
};
