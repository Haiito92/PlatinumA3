// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIGroupSubsystem.generated.h"

USTRUCT()
struct FAIGroupPawnData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UAIBehaviour* CurrentBehaviour;
};


class AAIGroupPawn;
class UAIBehaviour;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIGroupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TArray<AAIGroupPawn*> Pawns;
	TArray<FAIGroupPawnData> PawnDatas;
	
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
	FTimerHandle GroupUpdateTimerHandle;
	void GroupUpdate();
	
	UAIBehaviour* FindFirstValidBehaviour(AAIGroupPawn* Pawn)const;
#pragma endregion 
};
