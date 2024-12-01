// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PLATINUMA3_API ULoadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString LoadedLevel;
};
