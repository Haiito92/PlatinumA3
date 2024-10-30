// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "WoolsomeCharactersSettings.generated.h"

class USheepCharacterData;
/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName ="Woolsome Characters Settings"))
class PLATINUMA3_API UWoolsomeCharactersSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="SheepCharacter")
	TSoftObjectPtr<USheepCharacterData> SheepCharacterData;
};
