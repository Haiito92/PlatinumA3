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

class UFleeFollowerComponent;
class UFleeLeaderComponent;

USTRUCT()
struct FFleeGroupData
{
	GENERATED_BODY()
public:
	~FFleeGroupData() = default;

	UPROPERTY()
	UFleeLeaderComponent* GroupLeader;

	UPROPERTY()
	FVector GroupDirection = FVector::ZeroVector;
	
	UPROPERTY()
	TArray<UFleeFollowerComponent*> Followers;
};

UCLASS()
class PLATINUMA3_API UFleeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
#pragma region UnrealDefaults
protected:
	virtual TStatId GetStatId() const override {return TStatId();};
	virtual void Tick(float DeltaTime) override;
	
#pragma endregion 
public:
	
	UFUNCTION()
	void InitSubsystem();

	UFUNCTION()
	void UpdateSubsystem(float InDeltaTime);


#pragma region FleeLeaders
public:
	
protected:
	UFUNCTION()
	void OnLeaderStartFlee(int LeaderIndex);
	
	UFUNCTION()
	void OnLeaderStopFlee(int LeaderIndex);

private:
	UPROPERTY()
	TArray<UFleeLeaderComponent*> FleeLeaderComponents;
	
	UPROPERTY()
	TMap<int, FFleeGroupData>ActiveFleeGroups;

#pragma endregion 
#pragma region FleeFollowers
protected:
	UFUNCTION()
	void OnFollowerEncounteredNewGroup(int InGroupLeaderIndex, int InFollowerIndex);
	UFUNCTION()
	void OnFollowerLostContactWithGroup(int InGroupLeaderIndex, int InFollowerIndex);

private:
	UPROPERTY()
	TArray<UFleeFollowerComponent*> FleeFollowerComponents;
#pragma endregion 
};
