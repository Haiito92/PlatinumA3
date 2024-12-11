// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CampaignGameMode.generated.h"

enum class EWoolStateClassID : uint8;
class AWoolStateCharacter;
enum class ECampaignGameStateID : uint8;
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

	// virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override;
#pragma endregion


#pragma region OnEndPlay
private:
	void UnbindToWorldSubsystemsEvents() const;
#pragma endregion 
	
#pragma region Players
	void FindPlayerStartsActors(TArray<ACampaignPlayerStart*>& InOutPlayerStarts) const;

	void SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts, TArray<AStateCharacter*>& InOutCharacters) const;

	void CreateAndInitPlayers() const;

	void FindAndInitSplineCamera(TArray<AStateCharacter*>& Characters) const;

	TSoftClassPtr<AStateCharacter> GetCampaignCharacterClassByInputType(EAutoReceiveInput::Type InputType) const;

	TEnumAsByte<EAutoReceiveInput::Type> GetPlayerByCharacterClassID(EWoolStateClassID InClassID) const;
#pragma endregion 	


#pragma region GameLoop
public:
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceivePauseGame();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamePausedEvent);
	UPROPERTY(BlueprintAssignable)
	FGamePausedEvent GamePausedEvent;
	
	UFUNCTION(BlueprintCallable)
	void UnpauseGame();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveUnpauseGame();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameUnpausedEvent);
	UPROPERTY(BlueprintAssignable)
	FGameUnpausedEvent GameUnpausedEvent;
	
	UFUNCTION(BlueprintCallable)
	void FinishGame(bool bInWon);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameFinishedEvent, bool, bWon);
	UPROPERTY(BlueprintAssignable)
	FGameFinishedEvent GameFinishedEvent;
	
public:
	UFUNCTION(BlueprintCallable)
	ECampaignGameStateID GetGameStateID() const;
	UFUNCTION(BlueprintCallable)
	void SetGameStateID(ECampaignGameStateID NewGameStateID);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ECampaignGameStateID GameStateID;

private:
	UFUNCTION()
	void OnNotEnoughSheepLeftEvent();

	UFUNCTION()
	void OnReachedSheepAmountEvent();

#pragma endregion 
};
