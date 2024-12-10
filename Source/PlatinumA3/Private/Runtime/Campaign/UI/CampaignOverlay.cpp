// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Campaign/UI/CampaignOverlay.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Runtime/Campaign/UI/EndGameMenu.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"

void UCampaignOverlay::NativeDestruct()
{
	ACampaignGameMode* CampaignGameMode = Cast<ACampaignGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	CampaignGameMode->GamePausedEvent.RemoveDynamic(this, &UCampaignOverlay::OnGamePauseEvent);
	CampaignGameMode->GameUnpausedEvent.RemoveDynamic(this, &UCampaignOverlay::OnGameUnpauseEvent);
}

void UCampaignOverlay::InitCampaignOverlay_Implementation()
{
	// UE_LOGFMT(LogTemp, Warning,"Init Campaign Overlay");
	// GEngine->AddOnScreenDebugMessage(-1,6.0f,
	// 	FColor::Turquoise, TEXT("Init Campaign Overlay"));
	ACampaignGameMode* CampaignGameMode = Cast<ACampaignGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	CampaignGameMode->GamePausedEvent.AddDynamic(this, &UCampaignOverlay::OnGamePauseEvent);
	CampaignGameMode->GameUnpausedEvent.AddDynamic(this, &UCampaignOverlay::OnGameUnpauseEvent);
}

void UCampaignOverlay::OnGamePauseEvent()
{
	UE_LOGFMT(LogTemp, Warning, "Overlay On Game Paused");
	SetVisibility(ESlateVisibility::Hidden);
}

void UCampaignOverlay::OnGameUnpauseEvent()
{
	UE_LOGFMT(LogTemp, Warning, "Overlay On Game Un Paused");
	SetVisibility(ESlateVisibility::Visible);
}
