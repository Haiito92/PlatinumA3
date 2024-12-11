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
	int LevelIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName = "NotName";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelSheepAmountToFinish = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextLevelIndex = 0;
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
	bool GetLevelInfosByIndex(const int InLevelIndex, FLevelInfos& InOutLevelInfo) const;
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
	void OpenLevelByIndex(const int InLevelIndex);

	UFUNCTION(BlueprintCallable)
	void OpenNextLevel();

	UFUNCTION(BlueprintCallable)
	void OpenLevel(const FLevelInfos& InLevelInfo);
protected:
	
#pragma endregion 
};
