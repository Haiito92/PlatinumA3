// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AIBehaviour.generated.h"

/**
 * 
 */

class AAIGroupPawn;

UCLASS()
class PLATINUMA3_API UAIBehaviour : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool CheckBehaviourValidity() const;
	virtual void ApplyBehaviour(AAIGroupPawn* Character) const;
};
