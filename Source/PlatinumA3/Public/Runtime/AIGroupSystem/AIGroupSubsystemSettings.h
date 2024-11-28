// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AIGroupSubsystemSettings.generated.h"

class UAIBehaviour;
/**
 * 
 */
UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="AIGroup Subsystem Settings"))
class PLATINUMA3_API UAIGroupSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Pawns")
	TSubclassOf<AAIGroupCharacter> PawnClass;

	UPROPERTY(Config, EditAnywhere, Category="Pool")
	unsigned int PoolStartSize = 1;
	
	UPROPERTY(Config, EditAnywhere, Category="Behaviors")
	TArray<TSubclassOf<UAIBehaviour>> BehaviorsClasses;
};
