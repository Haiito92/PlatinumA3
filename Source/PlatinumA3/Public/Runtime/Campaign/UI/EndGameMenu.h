// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameMenu.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class PLATINUMA3_API UEndGameMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitEndGameMenu();
};
