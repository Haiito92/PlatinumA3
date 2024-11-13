// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CampaignGameMode.generated.h"

class ASheepCharacter;
class ACampaignPlayerStart;
class AStateCharacter;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API ACampaignGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Defaults
private:
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Initialization
	void InitWorldSubsystems() const;
	void InitGameInstanceSubsystems() const;
#pragma endregion 
	
#pragma region Players
	void FindPlayerStartsActors(TArray<ACampaignPlayerStart*>& InOutPlayerStarts) const;

	TArray<AStateCharacter*> SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts) const;

	void CreateAndInitPlayers() const;

	void FindAndInitSplineCamera(TArray<AStateCharacter*>& Characters) const;

	TSubclassOf<AStateCharacter> GetCampaignCharacterClassByInputType(EAutoReceiveInput::Type InputType) const;
#pragma endregion 	
	
// #pragma region Game
// private:
// 	UPROPERTY()
// 	TArray<ASheepCharacter*> AllSheeps; 
//
// 	UPROPERTY()
// 	int NbSheepToSucceedLevel;
//
// 	UPROPERTY()
// 	int NbSheepLeft;
// 	
// 	UPROPERTY()
// 	int SheepSaved;
// 	
// public:
// 	void SetNbSheepToSucceedLevel(int Amount);
// 	void AddSavedSheep(int Value);
//
// private:
// 	
// 	void FindAllSheepsInWorld(TArray<ASheepCharacter*>& InOutSheeps);
// 	void SubscribeToSheepsEvents() const;
//
// 	UFUNCTION()
// 	void OnSheepKillEvent();
// 	
// 	void RemoveSheepLeft(int Value);
// 	
// 	void CheckWinOrLose() const;
// 	bool HasSavedEnoughSheeps() const;
//
// 	void Win() const;
// 	void Lose() const;
//
// public:
// 	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndGameEvent, bool, HasWon);
//
// 	UPROPERTY()
// 	FEndGameEvent EndGameEvent;
// #pragma endregion 
};
