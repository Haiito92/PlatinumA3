// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CampaignGameMode.generated.h"

enum class EGameStateID : uint8;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region Initialization
	void InitWorldSubsystems() const;
	void BindToWorldSubsystemsEvents() const;
	
	void InitGameInstanceSubsystems() const;
#pragma endregion


#pragma region OnEndPlay
private:
	void UnbindToWorldSubsystemsEvents() const;
#pragma endregion 
	
#pragma region Players
	void FindPlayerStartsActors(TArray<ACampaignPlayerStart*>& InOutPlayerStarts) const;

	TArray<AStateCharacter*> SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts) const;

	void CreateAndInitPlayers() const;

	void FindAndInitSplineCamera(TArray<AStateCharacter*>& Characters) const;

	TSubclassOf<AStateCharacter> GetCampaignCharacterClassByInputType(EAutoReceiveInput::Type InputType) const;
#pragma endregion 	


#pragma region GameLoop
public:
	UFUNCTION(BlueprintCallable)
	void FinishGame();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinishedEvent);
	UPROPERTY()
	FGameFinishedEvent GameFinishedEvent;
	
public:
	UFUNCTION(BlueprintCallable)
	EGameStateID GetGameStateID() const;
	UFUNCTION(BlueprintCallable)
	void SetGameStateID(EGameStateID NewGameStateID);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	EGameStateID GameStateID;

private:
	UFUNCTION()
	void OnReachedSheepAmountEvent();

#pragma endregion 
};
