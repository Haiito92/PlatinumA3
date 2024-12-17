// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatEndGameMenu.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PLATINUMA3_API UCheatEndGameMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void CheatEndGame();
};
