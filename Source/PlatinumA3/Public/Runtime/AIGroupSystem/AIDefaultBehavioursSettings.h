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
	float IdleRadius;
	
	UPROPERTY(Config, EditAnywhere, Category="Idle Behaviour")
	float DirectionChangeTime;

	UPROPERTY(Config, EditAnywhere, Category="Idle Behaviour")
	float WalkSpeed;

	UPROPERTY(Config, EditAnywhere, Category="Idle Behaviour")
	bool CanIdleWalkOffLedges;

	UPROPERTY(Config, EditAnywhere, Category="Escape Corner Behaviour")
	float EscapeDetectionDistance;

	UPROPERTY(Config, EditAnywhere, Category="Escape Corner Behaviour")
	float EscapeWalkSpeed;

	UPROPERTY(Config, EditAnywhere, Category="Escape Corner Behaviour")
	bool CanEscapeWalkOffLedges;

	UPROPERTY(Config, EditAnywhere, Category="Escape Corner Behaviour")
	float PostEscapeTime = 0.5f;
};
