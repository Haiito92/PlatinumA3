// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelection.h"


FReply UCharacterSelection::NativeOnAnalogValueChanged(const FGeometry& InGeometry,
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
			bLeftStickXActive = true; // Marquer comme actif
			OnJoystickMoved(DeviceId, AnalogValue); // Déclencher l'événement Blueprint
		}
		// Si on pousse vers la gauche
		else if (AnalogValue <= -ActivationThreshold && !bLeftStickXActive)
		{
			bLeftStickXActive = true;
			OnJoystickMoved(DeviceId, AnalogValue);
		}
		// Réinitialisation
		else if (FMath::Abs(AnalogValue) <= DeactivationThreshold && bLeftStickXActive)
		{
			bLeftStickXActive = false; // Réinitialiser l'état
		}
	}
	return Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
}

void UCharacterSelection::NativeConstruct()
{
	Super::NativeConstruct();
}





