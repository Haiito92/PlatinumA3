// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeFollowerComponent.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"

#pragma region UnrealDefaults

void UFleeSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSubsystem(DeltaTime);
}
#pragma endregion 
void UFleeSubsystem::InitSubsystem()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAIGroupCharacter::StaticClass(),FoundActors);

	FleeLeaderComponents.Init(nullptr, FoundActors.Num());
	for (int i = 0; i < FleeLeaderComponents.Num(); ++i)
	{
		UFleeLeaderComponent* LeaderComponent = FoundActors[i]->FindComponentByClass<UFleeLeaderComponent>();
		if(LeaderComponent == nullptr) return;
		LeaderComponent->Init(i);
		FleeLeaderComponents[i] = LeaderComponent;
		LeaderComponent->StartFleeEvent.AddDynamic(this, &UFleeSubsystem::OnLeaderStartFlee);
		LeaderComponent->StopFleeEvent.AddDynamic(this, &UFleeSubsystem::OnLeaderStopFlee);
	}

	FleeFollowerComponents.Init(nullptr, FoundActors.Num());
	for (int i = 0; i < FleeFollowerComponents.Num(); ++i)
	{
		UFleeFollowerComponent* FollowerComponent = FoundActors[i]->FindComponentByClass<UFleeFollowerComponent>();
		if(FollowerComponent == nullptr) return;
		FollowerComponent->Init(i);
		FleeFollowerComponents[i] = FollowerComponent;
		FollowerComponent->EncounteredNewGroupEvent.AddDynamic(this, &UFleeSubsystem::OnFollowerEncounteredNewGroup);
		FollowerComponent->LostContactWithGroupEvent.AddDynamic(this, &UFleeSubsystem::OnFollowerLostContactWithGroup);
	}
}

void UFleeSubsystem::UpdateSubsystem(float InDeltaTime)
{
	for (TTuple<int, FFleeGroupData>& Pair : ActiveFleeGroups)
	{
		Pair.Value.GroupDirection = FleeLeaderComponents[Pair.Key]->GetOwner()->GetActorForwardVector();
		
		for (UFleeFollowerComponent* Follower : Pair.Value.Followers)
		{
			FGroupFollowedData* Data = Follower->GetGroupFollowedDatas().Find(Pair.Key);
			Data->GroupDirection = Pair.Value.GroupDirection;
		}
	}
}

#pragma region FleeLeaders


void UFleeSubsystem::OnLeaderStartFlee(int LeaderIndex)
{
	ActiveFleeGroups.Add(LeaderIndex, {FleeLeaderComponents[LeaderIndex]});
}

void UFleeSubsystem::OnLeaderStopFlee(int LeaderIndex)
{
	FFleeGroupData* Data = ActiveFleeGroups.Find(LeaderIndex);
	// for (UFleeFollowerComponent* Follower : Data->Followers)
	// {
	// 	Follower->GetGroupFollowedDatas().Remove(LeaderIndex);
	// }
	ActiveFleeGroups.Remove(LeaderIndex);
}

#pragma endregion 

#pragma region FleeFollowers
void UFleeSubsystem::OnFollowerEncounteredNewGroup(int InGroupLeaderIndex, int InFollowerIndex)
{
	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	if(Data==nullptr) return;

	Data->Followers.Add(FleeFollowerComponents[InFollowerIndex]);
}

void UFleeSubsystem::OnFollowerLostContactWithGroup(int InGroupLeaderIndex, int InFollowerIndex)
{
	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	if(Data==nullptr) return;

	Data->Followers.Remove(FleeFollowerComponents[InFollowerIndex]);
}
#pragma endregion 
