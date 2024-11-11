// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RallySystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="Rally System Settings"))
class PLATINUMA3_API URallySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Rally Emitter")
	float SignalRadius;

	UPROPERTY(Config, EditAnywhere, Category="Rally Receiver")
	float RallyTime;
	
	UPROPERTY(Config, EditAnywhere, Category="Rally Behaviour")
	float RallySpeed;
};
