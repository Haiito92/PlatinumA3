// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatTeleportMenu.generated.h"

class UCheatsSubsystem;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FUITeleportInfo
{
	GENERATED_BODY()
	~FUITeleportInfo() = default;

	UPROPERTY(BlueprintReadOnly, Category="Teleport")
	uint8 Index = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="Teleport")
	FName ShortName = "";
};

UCLASS()
class PLATINUMA3_API UCheatTeleportMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitMenu();
	
protected:

	UFUNCTION(BlueprintCallable)
	void PrevTeleportInfos();
	UFUNCTION(BlueprintCallable)
	void NextTeleportInfos();

	void UpdateTeleportInfos(int IndexDelta);
	void GetTeleportInfos(int Index);

	UFUNCTION(BlueprintCallable)
	void CheatTeleport() const;
	
	UPROPERTY(BlueprintReadOnly, Category="Teleport Infos")
	FUITeleportInfo CurrentTeleportInfo;

	UPROPERTY()
	TObjectPtr<UCheatsSubsystem> CheatsSubsystem;
};
