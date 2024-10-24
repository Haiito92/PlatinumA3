// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "SimpleGameMode.generated.h"

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
class STATEMACHINE_API ASimpleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerStart* FindPlayerStartActorInArena();
};
