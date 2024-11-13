// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SheepCountUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PLATINUMA3_API USheepCountUI : public UUserWidget
{
	GENERATED_BODY()
#pragma region Defaults	
protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
#pragma endregion
protected:
	UPROPERTY(BlueprintReadOnly)
	int SheepCapturedCount;

	UFUNCTION()
	void OnSheepCapturedCountChangedEvent(unsigned int InSheepCapturedCount);
	
	UFUNCTION()
	void UpdateSheepCapturedCountUI(unsigned InSheepCapturedCount);
	
};
