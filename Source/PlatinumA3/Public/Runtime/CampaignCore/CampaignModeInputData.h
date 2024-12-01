// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CampaignModeInputData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UCampaignModeInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> FinishGameInputAction;
};
