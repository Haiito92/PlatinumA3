// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLATINUMA3_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region LocalPlayers
void CreateAndInitPlayers() const;
#pragma endregion 
};
