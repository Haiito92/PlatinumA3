// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/InputTriggerPressedFast.h"

#include "EnhancedPlayerInput.h"

ETriggerState UInputTriggerPressedFast::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                   FInputActionValue ModifiedValue, float DeltaTime)
{
	if(IsActuated(ModifiedValue))
	{
		if(!bAlreadyTriggered)
		{
			Timer += DeltaTime;
			if(ModifiedValue.GetMagnitude() > PressedFastThreshold)
			{
				if(Timer<= Delay)
				{
					bAlreadyTriggered = true;
					return ETriggerState::Triggered;
				}
			}
		}
	}else
	{
		Timer = 0.0f;
		bAlreadyTriggered = false;
	}
	
	return ETriggerState::None;
}
