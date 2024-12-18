// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIGroupSubsystem.generated.h"


class UAIGroupSubsystemSettings;
class AAIGroupPawn;
class AAIGroupCharacter;
class UAIBehaviour;

USTRUCT()
struct FAIGroupPawnData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UAIBehaviour> CurrentBehaviour;
};



/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIGroupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	const float SYSTEM_UPDATE_RATE = 0.015f;


#pragma region SystemInitialization
public:
	void InitSubsystem();

private:
	void InitBehaviours();
	void FindAllPawns();
	void InitAllPawns();
	void InitPawnDatas();
	void StartFirstPawnsBehavior();
#pragma endregion

#pragma region System Update

private:
	UPROPERTY()
	FTimerHandle GroupUpdateTimerHandle;
	void GroupUpdate();
	
	UAIBehaviour* FindFirstValidBehaviour(AAIGroupCharacter* Pawn)const;
#pragma endregion

#pragma region SystemPool
protected:
	void InitPawnPool(const int InStartPawnAmount);
private:
	UPROPERTY()
	FVector PoolLocation;
	
	UPROPERTY()
	TArray<AAIGroupCharacter*> Pawns;
	UPROPERTY()
	TArray<FAIGroupPawnData> PawnDatas;
#pragma endregion
	
	UPROPERTY()
	TArray<UAIBehaviour*> Behaviours;
	//Ajouter un default behavior ??

	UPROPERTY()
	const UAIGroupSubsystemSettings* Settings;
};
