// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "WoolSomeSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="WoolSome Settings"))
class PLATINUMA3_API UWoolSomeSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category="Level Infos",BlueprintReadWrite)
	TSoftObjectPtr<UDataTable> LevelInfosDataTable;

	UFUNCTION(BlueprintCallable)
	static const UWoolSomeSettings* GetWoolSomeSettings();
};

inline const UWoolSomeSettings* UWoolSomeSettings::GetWoolSomeSettings()
{
	return GetDefault<UWoolSomeSettings>();
}
