// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SheepPenSystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="Sheep Pen System Settings"))
class PLATINUMA3_API USheepPenSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Sheep")
	FName SheepTag;
	
	UPROPERTY(Config, EditAnywhere, Category="Sheep In Pen")
	float SheepSpeedInPen;

	UPROPERTY(Config, EditAnywhere, Category="Sheep In Pen")
	bool CanSheepWalkOffLedges;
};
