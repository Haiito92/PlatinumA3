// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WoolsomeGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLevelInfos : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 LevelIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName = "NotName";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 LevelSheepAmountToFinish = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NextLevelIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLevelName = "NotNextName";
};

UCLASS()
class PLATINUMA3_API UWoolsomeGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region Initialization
public:
	UFUNCTION(BlueprintCallable)
	void InitGameInstance();

protected:

	UPROPERTY(BlueprintReadOnly)
	bool bLoaded;
#pragma endregion

#pragma region LevelInfos
public:
	UFUNCTION(BlueprintCallable)
	bool GetLevelInfosByIndex(const uint8 InLevelIndex, FLevelInfos& InOutLevelInfo) const;
	UFUNCTION(BlueprintCallable)
	const FLevelInfos& GetCurrentLevelInfo() const;
protected:
	UPROPERTY()
	TArray<FLevelInfos> LevelInfos;
	
	UPROPERTY()
	FLevelInfos CurrentLevelInfo;
#pragma endregion

#pragma region Levels
public:
	UFUNCTION(BlueprintCallable)
	void OpenLevelByIndex(const uint8 InLevelIndex);

	UFUNCTION(BlueprintCallable)
	void OpenNextLevel();

	UFUNCTION(BlueprintCallable)
	void OpenLevel(const FLevelInfos& InLevelInfo);
protected:
	
#pragma endregion 
};
