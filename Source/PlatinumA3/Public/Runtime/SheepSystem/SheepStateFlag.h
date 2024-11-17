// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType, meta=(BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESheepStateFlag
{
	None = 0 UMETA(Hidden),
	Free = 1,
	Captured = 2,
	Dead = 4,
 
	Alive = Captured + Free UMETA(Hidden)
};
ENUM_CLASS_FLAGS(ESheepStateFlag);
