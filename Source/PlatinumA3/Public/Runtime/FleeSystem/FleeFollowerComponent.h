// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FleeLeaderComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "FleeFollowerComponent.generated.h"


class UFleeLeaderComponent;

USTRUCT()
struct FGroupFollowedData
{
	GENERATED_BODY()
public:
	~FGroupFollowedData() = default;

	UPROPERTY()
	FVector GroupDirection = FVector::ZeroVector;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UFleeFollowerComponent : public USphereComponent
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults

public:
	// Sets default values for this component's properties
	UFleeFollowerComponent();

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region FleeFollower
public:
	UFUNCTION()
	void Init(int InFollowerIndex);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartFollowFleeEvent);
	UPROPERTY()
	FStartFollowFleeEvent StartFollowFleeEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopFollowFleeEvent);
	UPROPERTY()
	FStopFollowFleeEvent StopFollowFleeEvent;
	
	UFUNCTION()
	int GetFollowerIndex()const;

	UFUNCTION()
	bool GetFollowFleeing() const;
protected:
	
	UFUNCTION()
	void StartFollowFlee();
	UFUNCTION()
	void StopFollowFlee();
private:	

	UPROPERTY()
	int FollowerIndex;
	
	UPROPERTY()
	bool bFollowFleeing;
#pragma endregion

#pragma region GroupsFollowed
public:
	UFUNCTION()
	TMap<int,FGroupFollowedData>& GetGroupFollowedDatas();

	UFUNCTION()
	bool FollowsGroup(const int InGroupLeaderIndex) const;
	
	UFUNCTION()
	void AddGroupFollowed(const int InGroupLeaderIndex);

	UFUNCTION()
	void RemoveGroupFollowed(const int InGroupLeaderIndex);
private:
	UPROPERTY()
	TMap<int, FGroupFollowedData> GroupFollowedDatas;
#pragma endregion 
};

