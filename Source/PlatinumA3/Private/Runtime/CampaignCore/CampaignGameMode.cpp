// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Characters/StateCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/CampaignCore/CampaignPlayerStart.h"
#include "Runtime/Sheep/SheepCharacter.h"

#pragma region Defaults
void ACampaignGameMode::BeginPlay()
{
	Super::BeginPlay();
	//Spawn Players
	CreateAndInitPlayers();
	
	TArray<ACampaignPlayerStart*> PlayerStarts;
	FindPlayerStartsActors(PlayerStarts);
	SpawnCharacters(PlayerStarts);

	//Setup Game
	FindAllSheepsInWorld(AllSheeps);	
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
		TSubclassOf<AStateCharacter> CharacterClass = GetCampaignCharacterClassByInputType(InputType);
		if(CharacterClass == nullptr) continue;
		
		AStateCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AStateCharacter>(
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

TSubclassOf<AStateCharacter> ACampaignGameMode::GetCampaignCharacterClassByInputType(
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

	RemoveSheepLeft(Value);
}

void ACampaignGameMode::FindAllSheepsInWorld(TArray<ASheepCharacter*>& InOutSheeps)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASheepCharacter::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ASheepCharacter* SheepCharacter = Cast<ASheepCharacter>(FoundActor);
		if(SheepCharacter == nullptr) continue;

		InOutSheeps.Add(SheepCharacter);
	}
}

void ACampaignGameMode::SubscribeToSheepsEvents() const
{
	for (ASheepCharacter* Sheep : AllSheeps)
	{
		if(Sheep == nullptr) continue;
		Sheep->KillEvent.AddDynamic(this, &ACampaignGameMode::OnSheepKillEvent);
	}
}

void ACampaignGameMode::OnSheepKillEvent()
{
	RemoveSheepLeft(-1);
}

void ACampaignGameMode::RemoveSheepLeft(int Value)
{
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