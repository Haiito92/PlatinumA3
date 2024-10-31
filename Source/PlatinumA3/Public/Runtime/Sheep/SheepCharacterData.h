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
	UPROPERTY(EditAnywhere, Category="SheepDefault")
	float SheepWalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="Rally")
	float RallyTime;

	UPROPERTY(EditAnywhere, Category="Rally")
	float SheepRallySpeed;
	
	UPROPERTY(EditAnywhere, Category="Flee")
	TSubclassOf<AActor> ActorClassToFleeFrom;

	UPROPERTY(EditAnywhere, Category="Flee")
	float DetectionRadius;
	
	UPROPERTY(EditAnywhere, Category="Flee")
	float FleeingDistance;

	UPROPERTY(EditAnywhere, Category="Flee")
	float SheepFleeSpeed;
};
