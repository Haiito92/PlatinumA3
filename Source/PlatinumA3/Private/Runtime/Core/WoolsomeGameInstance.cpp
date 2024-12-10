// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Core/WoolsomeGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/WoolSomeSettings.h"
#include "Runtime/Loading/LoadingSubsystem.h"

void UWoolsomeGameInstance::InitGameInstance()
{
	const UWoolSomeSettings* Settings = GetDefault<UWoolSomeSettings>();
	if(Settings == nullptr) return;
	
	const UDataTable* LevelInfosDataTable = Settings->LevelInfosDataTable.LoadSynchronous();
	if(LevelInfosDataTable == nullptr) return;
	
	LevelInfos.Empty();

	const TArray<FName> RowNames = LevelInfosDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FLevelInfos* LevelInfo = LevelInfosDataTable->FindRow<FLevelInfos>(RowName,"");
		if(LevelInfo == nullptr) continue;
		LevelInfos.Add((*LevelInfo));
	}

	if(LevelInfos.Num() > 0)
	{
		CurrentLevelInfo = LevelInfos[0];
		bLoaded = true;
		return;
	}

	bLoaded = false;
}

bool UWoolsomeGameInstance::GetLevelInfosByIndex(const int InLevelIndex, FLevelInfos& InOutLevelInfo) const
{
	if(InLevelIndex < 0 || !LevelInfos.IsValidIndex(InLevelIndex)) return false;

	InOutLevelInfo = LevelInfos[InLevelIndex];
	return true;
}

const FLevelInfos& UWoolsomeGameInstance::GetCurrentLevelInfo() const
{
	return CurrentLevelInfo;
}

void UWoolsomeGameInstance::OpenLevelByIndex(const int InLevelIndex)
{
	FLevelInfos LevelInfo={};
	if(!GetLevelInfosByIndex(InLevelIndex, LevelInfo)) return;

	OpenLevel(LevelInfo);
}

void UWoolsomeGameInstance::OpenNextLevel()
{
	OpenLevelByIndex(CurrentLevelInfo.NextLevelIndex);
}

void UWoolsomeGameInstance::OpenLevel(const FLevelInfos& InLevelInfo)
{
	ULoadingSubsystem* LoadingSubsystem = GetSubsystem<ULoadingSubsystem>();
	if(LoadingSubsystem == nullptr) return;
	
	LoadingSubsystem->LoadedLevel = InLevelInfo.LevelName.ToString();
	CurrentLevelInfo = InLevelInfo;

	UGameplayStatics::OpenLevel(GetWorld(), "LoadingScreenLevel");
}
