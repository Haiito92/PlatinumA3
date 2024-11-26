// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CheatsSubsystem.generated.h"

class ACheatTeleportPoint;
/**
 * 
 */
UCLASS()

class PLATINUMA3_API UCheatsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void InitSubsystem();

#pragma region CheatTeleport

public:
	const TArray<TObjectPtr<ACheatTeleportPoint>>& GetCheatTeleportPoints() const;
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<ACheatTeleportPoint>> CheatTeleportPoints;
	
#pragma endregion
};
