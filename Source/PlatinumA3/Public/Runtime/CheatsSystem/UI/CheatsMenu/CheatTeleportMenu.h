// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatTeleportMenu.generated.h"

/**
 * 
 */
USTRUCT()
struct FUITeleportInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Teleport")
	FName ShortName;
};

UCLASS()
class PLATINUMA3_API UCheatTeleportMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	void GetTeleportInfos(int Index);
	
	UPROPERTY()
	FUITeleportInfo CurrentTeleportInfo;

	
};
