// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/MainMenu/CharacterSelectionWidget.h"


FReply UCharacterSelectionWidget::NativeOnAnalogValueChanged(const FGeometry& InGeometry,
	const FAnalogInputEvent& InAnalogEvent)
{
	FKey Key = InAnalogEvent.GetKey();
	int DeviceId = InAnalogEvent.GetInputDeviceId().GetId();
	float AnalogValue = InAnalogEvent.GetAnalogValue();

	const float ActivationThreshold = 1.0f;
	const float DeactivationThreshold = 0.2f;
	
	if (Key == EKeys::Gamepad_LeftX) 
	{
		if (AnalogValue >= ActivationThreshold && !bLeftStickXActive)
		{
			bLeftStickXActive = true;
			OnJoystickMoved(DeviceId, AnalogValue);
		}

		else if (AnalogValue <= -ActivationThreshold && !bLeftStickXActive)
		{
			bLeftStickXActive = true;
			OnJoystickMoved(DeviceId, AnalogValue);
		}

		else if (FMath::Abs(AnalogValue) <= DeactivationThreshold && bLeftStickXActive)
		{
			bLeftStickXActive = false;
		}
	}
	return Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
}

void UCharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UCharacterSelectionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Q)
	{
		OnJoystickMoved(0, -1.0f);
	}
	else if (Key == EKeys::D)
	{
		OnJoystickMoved(0, 1.0f);
	}else if (Key == EKeys::Left)
	{
		OnJoystickMoved(1, -1.0f);
	}else if (Key == EKeys::Right){
		OnJoystickMoved(1, 1.0f);
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}





