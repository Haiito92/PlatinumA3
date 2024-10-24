// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputMappingContext.h"

#include "SmashCharacterSettings.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Smash Character Settings"))
class STATEMACHINE_API USmashCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	float InputMoveXThreshold = 0.1f;
};