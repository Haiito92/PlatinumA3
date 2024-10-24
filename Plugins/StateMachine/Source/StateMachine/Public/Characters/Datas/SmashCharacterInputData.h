// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SmashCharacterInputData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class STATEMACHINE_API USmashCharacterInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Action", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Action", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs|Action", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> HoldingAction;
};
