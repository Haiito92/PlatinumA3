// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/AIGroupSystem/AIDefaultBehavioursSettings.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/Core/WoolSomeSettings.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"
#include "Runtime/RallySystem/RallySystemSettings.h"
#include "Runtime/SheepSystem/SheepPenSystemSettings.h"
#include "DevSettingsBlueprintLibrary.generated.h"


/**
 * 
 */
UCLASS()
class PLATINUMA3_API UDevSettingsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const UAIDefaultBehavioursSettings* GetAIDefaultBehavioursSettings()
	{
		return GetDefault<UAIDefaultBehavioursSettings>();
	}

	// UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	// static const UAIGroupSubsystemSettings* GetAIGroupSubsystemSettings()
	// {
	// 	return GetDefault<UAIGroupSubsystemSettings>();
	// }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const UCampaignModeSettings* GetCampaignModeSettings()
	{
		return GetDefault<UCampaignModeSettings>();
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const UFleeSystemSettings* GetFleeSystemSettings()
	{
		return GetDefault<UFleeSystemSettings>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const URallySystemSettings* GetRallySystemSettings()
	{
		return GetDefault<URallySystemSettings>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const USheepPenSystemSettings* GetSheepPenSystemSettings()
	{
		return GetDefault<USheepPenSystemSettings>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Dev Settings")
	static const UWoolSomeSettings* GetWoolsomeSettings()
	{
		return GetDefault<UWoolSomeSettings>();
	}
};
