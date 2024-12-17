// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/UI/CheatsMenu/CheatEndGameMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"
#include "Runtime/Core/WoolsomeGameInstance.h"

void UCheatEndGameMenu::InitEndGameMenu()
{
	const UWoolsomeGameInstance* GameInstance = 
	Cast<UWoolsomeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance == nullptr) return;

	
	const TArray<FLevelInfos>& LevelInfos = GameInstance->GetAllLevelInfos();

	for (const FLevelInfos& LevelInfo : LevelInfos)
	{
		LevelNames.Add(LevelInfo.LevelName);
	}

	GetLevelInfo(0);
}

void UCheatEndGameMenu::PrevLevelInfos()
{
	UpdateLevelInfo(-1);
}

void UCheatEndGameMenu::NextLevelInfos()
{
	UpdateLevelInfo(1);
}

void UCheatEndGameMenu::UpdateLevelInfo(int InIndexDelta)
{
	if(LevelNames.Num() == 0) return;

	int NewIndex = (CurrentIndex + InIndexDelta) % LevelNames.Num();
	if(NewIndex < 0) NewIndex = LevelNames.Num() - 1;

	GetLevelInfo(NewIndex);
}

void UCheatEndGameMenu::GetLevelInfo(int InIndex)
{
	if(LevelNames.Num() == 0) return;

	CurrentIndex = InIndex;
	CurrentName = LevelNames[CurrentIndex];
}

void UCheatEndGameMenu::CheatEndGame() const
{
	UWoolsomeGameInstance* GameInstance = 
	Cast<UWoolsomeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance == nullptr) return;

	GameInstance->OpenLevelByIndex(CurrentIndex);
}
