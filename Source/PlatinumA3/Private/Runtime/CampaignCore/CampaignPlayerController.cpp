// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"
#include "Runtime/CampaignCore/CampaignModeInputData.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/CampaignCore/CampaignGameStateID.h"

void ACampaignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CampaignGameMode = Cast<ACampaignGameMode>(GetWorld()->GetAuthGameMode());
	
	
}

void ACampaignPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	BindToInputs();
}

void ACampaignPlayerController::BindToInputs()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent == nullptr) return;
	
	const UCampaignModeSettings* CampaignModeSettings = GetDefault<UCampaignModeSettings>();
	if(CampaignModeSettings == nullptr) return;

	const UCampaignModeInputData* InputData = CampaignModeSettings->CampaignModeInputData.LoadSynchronous();
	if(InputData == nullptr) return;

	
	EnhancedInputComponent->BindAction(InputData->FinishGameInputAction, ETriggerEvent::Completed,
		this, &ACampaignPlayerController::OnFinishGameInputAction);

}

void ACampaignPlayerController::OnFinishGameInputAction()
{
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	2.0f,
	// 	FColor::Turquoise,
	// 	TEXT("Finish Game"));
	if(CampaignGameMode->GetGameStateID() != ECampaignGameStateID::Finishable) return;

	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Turquoise,
		TEXT("Finish Game"));
	
	CampaignGameMode->FinishGame();
}
