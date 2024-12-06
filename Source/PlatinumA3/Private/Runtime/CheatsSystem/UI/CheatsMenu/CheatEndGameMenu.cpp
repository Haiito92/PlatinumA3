// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/UI/CheatsMenu/CheatEndGameMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"

void UCheatEndGameMenu::CheatEndGame()
{
	ACampaignGameMode* CampaignGameMode = Cast<ACampaignGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(CampaignGameMode == nullptr)return;

	CampaignGameMode->FinishGame();
}
