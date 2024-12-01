// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CampaignPlayerController.generated.h"

class ACampaignGameMode;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API ACampaignPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	void BindToInputs();
	
	UFUNCTION()
	void OnFinishGameInputAction();

	UPROPERTY()
	TObjectPtr<ACampaignGameMode> CampaignGameMode;
};
