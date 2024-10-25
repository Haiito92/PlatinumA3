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
	NONE UMETA(DisplayName = "None"),
	BERGER UMETA(DisplayName = "Berger"),
	CHIEN UMETA(DisplayName = "Chien")
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
