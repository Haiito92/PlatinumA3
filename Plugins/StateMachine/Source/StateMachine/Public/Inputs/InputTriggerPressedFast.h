// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTriggerPressedFast.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName= "Pressed Fast", NotInputConfigurable="true"))
class STATEMACHINE_API UInputTriggerPressedFast : public UInputTrigger
{
	GENERATED_BODY()

protected:
	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Trigger Settings", meta=(DisplayThumbnail="false"))
	float PressedFastThreshold = 0.8f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Trigger Settings", meta=(DisplayThumbnail="false"))
	float Delay = 0.5f;

private:
	float Timer = 0.f;

	bool bAlreadyTriggered = false;
};
