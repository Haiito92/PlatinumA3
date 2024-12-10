// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CampaignOverlay.generated.h"

class UEndGameMenu;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UCampaignOverlay : public UUserWidget
{
	GENERATED_BODY()

#pragma region Unreal Defaults
	virtual void NativeDestruct() override;
#pragma endregion 
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitCampaignOverlay();
	

protected:
	UFUNCTION()
	void OnGamePauseEvent();
	UFUNCTION()
	void OnGameUnpauseEvent();
};
