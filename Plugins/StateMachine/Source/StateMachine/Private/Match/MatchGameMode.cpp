// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"

#include "Arena/ArenaPlayerStart.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "Characters/Datas/SmashCharacterInputData.h"  // Adjust according to your folder structure

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartPoints;

	FindPlayerStartActorsInArena(PlayerStartPoints);
	SpawnCharacters(PlayerStartPoints);

}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);

		if (ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* MappingContext = LoadInputMappingContextFromConfig();
	
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);

		if(SmashCharacterClass == nullptr) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);


		if(NewCharacter == nullptr) continue;

		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContext = MappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(NewCharacter);
	}
}


TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	switch(InputType)
	{
		case EAutoReceiveInput::Type::Player0:
			return SmashCharacterClassP0;

		case EAutoReceiveInput::Type::Player1:
			return SmashCharacterClassP1;

		case EAutoReceiveInput::Type::Player2:
			return SmashCharacterClassP2;

		case EAutoReceiveInput::Type::Player3:
			return SmashCharacterClassP3;

		default:
			return nullptr;
	}
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();

	if(!CharacterSettings) return nullptr;

	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();

	if(!CharacterSettings) return nullptr;

	return CharacterSettings->InputMappingContext.LoadSynchronous();
}
