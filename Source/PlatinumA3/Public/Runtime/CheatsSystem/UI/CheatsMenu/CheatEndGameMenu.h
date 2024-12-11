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

public:
	UFUNCTION(BlueprintCallable)
	void InitEndGameMenu();
	
protected:

	UFUNCTION(BlueprintCallable)
	void PrevLevelInfos();
	UFUNCTION(BlueprintCallable)
	void NextLevelInfos();
	
	void UpdateLevelInfo(int InIndexDelta);
	void GetLevelInfo(int InIndex);
	
	UFUNCTION(BlueprintCallable)
	void CheatEndGame() const;

	UPROPERTY()
	TArray<FName> LevelNames;

	UPROPERTY(BlueprintReadOnly)
	FName CurrentName;
	
	UPROPERTY()
	int CurrentIndex = 0;
};
