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

	virtual bool CheckBehaviourValidity() const override;
	virtual void ApplyBehaviour(AAIGroupPawn* Character) const override;
};
