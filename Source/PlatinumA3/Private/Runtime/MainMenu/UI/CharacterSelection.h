// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelection.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FJoystickMovedDelegate, FInputDeviceId, DeviceId, FKey, Key, float, Value);


UCLASS()
class PLATINUMA3_API UCharacterSelection : public UUserWidget
{
	GENERATED_BODY()

	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;
	
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:

	UPROPERTY(BlueprintReadWrite, Category = "Joystick")
	bool bLeftStickXActive = false;

	UPROPERTY(BlueprintReadWrite, Category = "Joystick")
	bool bRightStickXActive = false;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Joystick")
	void OnJoystickMoved(int DeviceId, float Value);
	
};
