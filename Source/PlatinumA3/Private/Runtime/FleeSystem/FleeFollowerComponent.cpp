// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeFollowerComponent.h"

#include "Components/SphereComponent.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
UFleeFollowerComponent::UFleeFollowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFleeFollowerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFleeFollowerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UFleeFollowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#pragma endregion

#pragma region FleeFollower
void UFleeFollowerComponent::Init(int InFollowerIndex)
{
	FollowerIndex = InFollowerIndex;
	// const UFleeSystemSettings* Settings = GetDefault<UFleeSystemSettings>();
	// if(Settings!= nullptr)
	// {
	// 	SphereRadius = Settings->FollowFleeDetectionRadius;
	// }
}

int UFleeFollowerComponent::GetFollowerIndex() const
{
	return FollowerIndex;
}

bool UFleeFollowerComponent::GetFollowFleeing() const
{
	return bFollowFleeing;
}

void UFleeFollowerComponent::StartFollowFlee()
{
	bFollowFleeing = true;
	StartFollowFleeEvent.Broadcast();
}

void UFleeFollowerComponent::StopFollowFlee()
{
	bFollowFleeing = false;
	StopFollowFleeEvent.Broadcast();
}
#pragma endregion 
#pragma region GroupsFollowed


TMap<int, FGroupFollowedData>& UFleeFollowerComponent::GetGroupFollowedDatas()
{
	return GroupFollowedDatas;
}

bool UFleeFollowerComponent::FollowsGroup(const int InGroupLeaderIndex) const
{
	return GroupFollowedDatas.Contains(InGroupLeaderIndex);
}

void UFleeFollowerComponent::AddGroupFollowed(const int InGroupLeaderIndex)
{
	GroupFollowedDatas.Add(InGroupLeaderIndex);

	if(GroupFollowedDatas.Num() == 1) StartFollowFlee();
}

void UFleeFollowerComponent::RemoveGroupFollowed(const int InGroupLeaderIndex)
{
	GroupFollowedDatas.Remove(InGroupLeaderIndex);

	if(GroupFollowedDatas.Num() == 0) StopFollowFlee();
}
#pragma endregion 
