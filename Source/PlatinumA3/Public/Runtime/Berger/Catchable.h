// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Catchable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCatchable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLATINUMA3_API ICatchable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Catchable Interface")
	UPrimitiveComponent* Catch();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Catchable Interface")
	void Drop();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Catchable Interface")
	void Launch(bool IsSimulatingPhysic, FName CollisionProfilName, float TransTime);
};