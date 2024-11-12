// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "AISheepInPenBehaviour.generated.h"

class USheepComponent;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAISheepInPenBehaviour : public UAIBehaviour
{
	GENERATED_BODY()
#pragma region BehaviourDefaults
	virtual void InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns) override;

	virtual bool CheckBehaviourValidity(AAIGroupCharacter* Pawn) const override;

	virtual void BehaviourEntry(AAIGroupCharacter* Pawn) override;
	virtual void BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime) override;
	virtual void BehaviourExit(AAIGroupCharacter* Pawn) override;
#pragma endregion

private:
	UPROPERTY()
	TArray<USheepComponent*> SheepComponents;
};
