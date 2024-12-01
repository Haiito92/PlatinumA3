// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Biteable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UBiteable : public UInterface
{
	GENERATED_BODY()

	
};

/**
 * 
 */
class PLATINUMA3_API IBiteable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Biteable Interface")
	void Bite(float StunTimer);
	
};
