// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FleeSystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="Flee System Settings"))
class PLATINUMA3_API UFleeSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditAnywhere, Category="Flee Behaviour")
	FName TagToFleeFrom;

	UPROPERTY(Config, EditAnywhere, Category="Flee Behaviour")
	float FleeDetectionRadius;
	
	UPROPERTY(Config, EditAnywhere, Category="Flee Behaviour")
	float FleeSpeed;

	UPROPERTY(Config, EditAnywhere, Category="Flee Behaviour")
	bool CanFleeWalkOffLedges;

	UPROPERTY(Config, EditAnywhere, Category="Flee Behaviour")
	float PostFleeTime = 0.3f;
	
	UPROPERTY(Config,EditAnywhere, Category="Follow Flee Behaviour")
	float FollowFleeDetectionRadius;

	UPROPERTY(Config, EditAnywhere, Category="Follow Flee Behaviour")
	float FollowFleeSpeed;

	UPROPERTY(Config, EditAnywhere, Category="Follow Flee Behaviour")
	bool CanFollowFleeWalkOffLedges;

	UPROPERTY(Config, EditAnywhere, Category="Bark")
	float BarkCooldownMin = 3.0f;
	
	UPROPERTY(Config, EditAnywhere, Category="Bark")
	float BarkCooldownMax = 6.0f;
};
