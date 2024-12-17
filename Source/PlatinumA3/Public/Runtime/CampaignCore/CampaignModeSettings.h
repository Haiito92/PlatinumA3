// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CampaignModeSettings.generated.h"

class UCampaignModeInputData;
class AStateCharacter;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Campaign Mode Settings"))
class PLATINUMA3_API UCampaignModeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="PlayerControler")
	TSoftObjectPtr<UCampaignModeInputData> CampaignModeInputData;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Characters")
	TSoftClassPtr<AStateCharacter> CampaignCharacterClassP0;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Characters")
	TSoftClassPtr<AStateCharacter> CampaignCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Characters")
	TEnumAsByte<EAutoReceiveInput::Type> PlayerPlayingShepherd;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Characters")
	TEnumAsByte<EAutoReceiveInput::Type> PlayerPlayingDog;
	
	UFUNCTION(BlueprintCallable)
	static const UCampaignModeSettings* GetCampaignModeSettings();
};

inline const UCampaignModeSettings* UCampaignModeSettings::GetCampaignModeSettings()
{
	return GetDefault<UCampaignModeSettings>();
}
