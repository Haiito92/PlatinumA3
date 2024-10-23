// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class AArenaPlayerStart;
class ASmashCharacter;
/**
 * 
 */


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	CHARACTERA UMETA(DisplayName = "Character A"),
	CHARACTERB UMETA(DisplayName = "Character B"),
	CHARACTERC UMETA(DisplayName = "Character C"),
	CHARACTERD UMETA(DisplayName = "Character D")
};

UCLASS()
class STATEMACHINE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<ASmashCharacter*> CharactersInsideArena;

	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP3;


private:
	UFUNCTION()
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);

	UFUNCTION()
	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);
	
	UFUNCTION()
	TSubclassOf<ASmashCharacter> GetCharacterClassFromCharacterType(ECharacterType CharacterType) const;

	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
};
