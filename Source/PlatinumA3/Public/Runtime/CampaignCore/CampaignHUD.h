// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CampaignHUD.generated.h"

class UEndGameMenu;
class UCampaignOverlay;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API ACampaignHUD : public AHUD
{
	GENERATED_BODY()
#pragma region Unreal Defaults

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion 
	
public:
	
	UFUNCTION()
	void InitCampaignHUD();
	
protected:
	UFUNCTION()
	void OnGameFinishableEvent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveGameFinishable();
	
	UFUNCTION()
	void OnGameFinishedEvent(bool bInWon);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void ReceiveGameFinished(bool bInWon);
};
