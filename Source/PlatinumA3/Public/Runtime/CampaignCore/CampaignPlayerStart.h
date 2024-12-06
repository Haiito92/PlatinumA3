// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CampaignPlayerStart.generated.h"

enum class EWoolStateClassID : uint8;
class AWoolStateCharacter;

UCLASS()
class PLATINUMA3_API ACampaignPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UFUNCTION()
	TSubclassOf<AWoolStateCharacter> GetClassToSpawn() const;
	UFUNCTION()
	void SetClassToSpawn(TSubclassOf<AWoolStateCharacter> InClassToSpawn);

	UFUNCTION()
	EWoolStateClassID GetWoolStateClassID()const;
	UFUNCTION()
	void SetWoolStateClassID(EWoolStateClassID InClassID);
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWoolStateCharacter> ClassToSpawn;

	UPROPERTY(EditAnywhere)
	EWoolStateClassID WoolStateClassID;
	
};
