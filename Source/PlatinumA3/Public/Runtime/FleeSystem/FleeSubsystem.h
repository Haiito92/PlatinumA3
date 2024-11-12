// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FleeSubsystem.generated.h"

// /**
//  * 
//  */
// USTRUCT()
// struct FFleeLeaderData
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY()
// 	int LeaderPawnIndex;
// 	const FVector& GetLeaderLocation() const;
// 	const FVector& GetLeaderForwardVector() const;
// private:
// 	UPROPERTY()
// 	FVector& LeaderLocation;
// 	UPROPERTY()
// 	FVector& LeaderForwardVector;
// };

class UFleeLeaderComponent;

UCLASS()
class PLATINUMA3_API UFleeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void InitSubsystem();

	UFUNCTION()
	TMap<int,UFleeLeaderComponent*>& GetCurrentFleeLeaders();
private:
	UPROPERTY()
	TMap<int,UFleeLeaderComponent*> CurrentFleeLeaders;
// public:
// 	UFUNCTION()
// 	TMap<int,FFleeLeaderData>& GetLeadersData() const;
// private:
// 	UPROPERTY()
// 	TMap<int,FFleeLeaderData> LeadersData;
};
