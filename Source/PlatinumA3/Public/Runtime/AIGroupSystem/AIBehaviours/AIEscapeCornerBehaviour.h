// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AIEscapeCornerBehaviour.generated.h"

class UAIDefaultBehavioursSettings;
/**
 * 
 */
USTRUCT()
struct FEscapeCornerPawnData
{
	GENERATED_BODY()
public:
	~FEscapeCornerPawnData() = default;

	FVector EscapeDirection = FVector::ZeroVector;
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
	const UAIDefaultBehavioursSettings* Settings;
	
	UPROPERTY()
	TArray<FEscapeCornerPawnData> EscapeDatas;
};
