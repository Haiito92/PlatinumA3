// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/PlatinumA3Character.h"
#include "Runtime/CampaignCore/CampaignPlayerStart.h"
#pragma region Defaults
void ACampaignGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Players
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

		APlatinumA3Character* NewCharacter = GetWorld()->SpawnActorDeferred<APlatinumA3Character>(
			APlatinumA3Character::StaticClass(),
			CampaignPlayerStart->GetTransform()
			);

		if(NewCharacter == nullptr) continue;

		NewCharacter->AutoPossessPlayer = CampaignPlayerStart->AutoReceiveInput;
		NewCharacter->FinishSpawning(CampaignPlayerStart->GetTransform());
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