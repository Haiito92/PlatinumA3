// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "../../../../../Plugins/StateMachine/Source/StateMachine/Public/Characters/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/CampaignCore/CampaignPlayerStart.h"

#pragma region Defaults
void ACampaignGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Players
	CreateAndInitPlayers();
	
	TArray<ACampaignPlayerStart*> PlayerStarts;
	FindPlayerStartsActors(PlayerStarts);
	SpawnCharacters(PlayerStarts);
	
	//Reset GameValue
	SheepSaved = 0;
	SetNbSheepToSucceedLevel(1);
}
#pragma endregion

#pragma region Players
void ACampaignGameMode::FindPlayerStartsActors(TArray<ACampaignPlayerStart*>& InOutPlayerStarts) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampaignPlayerStart::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ACampaignPlayerStart* CampaignPlayerStart = Cast<ACampaignPlayerStart>(FoundActor);

		if (CampaignPlayerStart == nullptr) continue;
		
		InOutPlayerStarts.Add(CampaignPlayerStart);
	}
}

void ACampaignGameMode::SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts) const
{
	
	
	for (ACampaignPlayerStart* CampaignPlayerStart : InPlayerStarts)
	{
		EAutoReceiveInput::Type InputType = CampaignPlayerStart->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> CharacterClass = GetCampaignCharacterClassByInputType(InputType);
		if(CharacterClass == nullptr) continue;
		
		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			CharacterClass,
			CampaignPlayerStart->GetTransform()
			);

		if(NewCharacter == nullptr) continue;

		NewCharacter->AutoPossessPlayer = CampaignPlayerStart->AutoReceiveInput;
		NewCharacter->FinishSpawning(CampaignPlayerStart->GetTransform());
	}
}

void ACampaignGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr) return;
	
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(LocalMultiplayerSubsystem == nullptr) return;
	
	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

TSubclassOf<ASmashCharacter> ACampaignGameMode::GetCampaignCharacterClassByInputType(
	EAutoReceiveInput::Type InInputType) const
{
	const UCampaignModeSettings* CampaignModeSettings = GetDefault<UCampaignModeSettings>();
	if(CampaignModeSettings == nullptr) return nullptr;
	
	switch (InInputType)
	{
	case EAutoReceiveInput::Player0:
		return CampaignModeSettings->CampaignCharacterClassP0;
	case EAutoReceiveInput::Player1:
		return CampaignModeSettings->CampaignCharacterClassP1;
	default: return nullptr;
	}
}


#pragma endregion

#pragma region Game
void ACampaignGameMode::SetNbSheepToSucceedLevel(int Amount)
{
	NbSheepToSucceedLevel = Amount;
}

void ACampaignGameMode::AddSavedSheep(int Value)
{
	SheepSaved += Value;

	NbSheepLeft -= Value;

	if(NbSheepLeft <= 0) CheckWinOrLose();
}

void ACampaignGameMode::CheckWinOrLose() const
{
	HasSavedEnoughSheeps() ? Win() : Lose();
}

bool ACampaignGameMode::HasSavedEnoughSheeps() const
{
	return SheepSaved > NbSheepToSucceedLevel;
}

void ACampaignGameMode::Win() const
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Emerald,
		TEXT("Win"));

	EndGameEvent.Broadcast(true);
}

void ACampaignGameMode::Lose() const
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Emerald,
		TEXT("Win"));

	EndGameEvent.Broadcast(false);
}
#pragma endregion 