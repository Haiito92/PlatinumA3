// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/MainMenu/MainMenuGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"

#pragma region UnrealDefaults
void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateAndInitPlayers();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Turquoise,
		TEXT("MM GM Begin play"));
	
}
#pragma endregion

#pragma region LocalPlayers

void AMainMenuGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::Menu);
}
#pragma endregion 