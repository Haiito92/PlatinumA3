// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatsMenu.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class PLATINUMA3_API UCheatsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitCheatsMenu();
};
