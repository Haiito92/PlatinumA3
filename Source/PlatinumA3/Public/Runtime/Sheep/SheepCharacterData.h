// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SheepCharacterData.generated.h"

/**
 * 
 */
UCLASS()
class PLATINUMA3_API USheepCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClassToFleeFrom;

	UPROPERTY(EditAnywhere)
	float DetectionRadius;
	
	UPROPERTY(EditAnywhere)
	float FleeingDistance;

	UPROPERTY(EditAnywhere)
	float SheepWalkSpeed;
};
