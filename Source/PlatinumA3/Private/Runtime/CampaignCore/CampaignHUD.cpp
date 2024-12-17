// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignHUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Campaign/UI/CampaignOverlay.h"
#include "Runtime/Campaign/UI/EndGameMenu.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"

#pragma region Unreal Defaults
void ACampaignHUD::BeginPlay()
{
	Super::BeginPlay();
	
	InitCampaignHUD();
}

void ACampaignHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ACampaignGameMode* CampaignGameMode = Cast<ACampaignGameMode>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(CampaignGameMode == nullptr) return;

	CampaignGameMode->GameFinishedEvent.RemoveDynamic(this, &ACampaignHUD::OnGameFinishedEvent);
}

#pragma endregion

void ACampaignHUD::InitCampaignHUD()
{
	if(UGameplayStatics::GetPlayerControllerID(PlayerOwner) != 0) return;
	
	ACampaignGameMode* CampaignGameMode = Cast<ACampaignGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(CampaignGameMode == nullptr) return;
	
	CampaignGameMode->GameFinishedEvent.AddDynamic(this, &ACampaignHUD::OnGameFinishedEvent);
}

void ACampaignHUD::OnGameFinishedEvent()
{
	ReceiveGameFinished();
}
