// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIGroupSubsystem.generated.h"


class AAIGroupPawn;
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
	UPROPERTY()
	TArray<AAIGroupPawn*> Pawns;
	UPROPERTY()
	TArray<FAIGroupPawnData> PawnDatas;

	UPROPERTY()
	TArray<UAIBehaviour*> Behaviours;
	//Ajouter un default behavior ??

#pragma region SystemInitialization
public:
	void InitSubsystem();

private:
	void InitBehaviours();
	void FindAllPawns();
	void InitAllPawns();
	void InitPawnDatas();
#pragma endregion

#pragma region System Update

private:
	UPROPERTY()
	FTimerHandle GroupUpdateTimerHandle;
	void GroupUpdate();
	
	UAIBehaviour* FindFirstValidBehaviour(AAIGroupPawn* Pawn)const;
#pragma endregion 
};
