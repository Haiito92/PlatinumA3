// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CampaignModeSettings.generated.h"

class APlatinumA3Character;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Campaign Mode Settings"))
class PLATINUMA3_API UCampaignModeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<APlatinumA3Character> CampaignCharacterClass;
};
