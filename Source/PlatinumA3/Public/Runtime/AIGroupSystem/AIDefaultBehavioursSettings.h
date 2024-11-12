// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AIDefaultBehavioursSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="AI Default Behaviours Settings"))
class PLATINUMA3_API UAIDefaultBehavioursSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Idle Behaviour")
	float DirectionChangeTime;

	UPROPERTY(Config, EditAnywhere, Category="Idle Behaviour")
	float WalkSpeed;
};
