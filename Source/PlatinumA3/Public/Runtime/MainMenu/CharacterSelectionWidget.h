// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLATINUMA3_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;
	
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnJoystickMoved(int DeviceId, float Value);

	bool bLeftStickXActive = false;
	
};
