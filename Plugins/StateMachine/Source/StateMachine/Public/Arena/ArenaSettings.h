// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Smash Arena Settings"))
class STATEMACHINE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()
};
