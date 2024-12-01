// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeBrainComponent.h"
#include "Runtime/FleeSystem/FleeFollowerComponent.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

#pragma region UnrealDefaults

void UFleeSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSubsystem(DeltaTime);
}
#pragma endregion 
void UFleeSubsystem::InitSubsystem()
{
	const UFleeSystemSettings* Settings = GetDefault<UFleeSystemSettings>();
	if(Settings == nullptr) return;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAIGroupCharacter::StaticClass(),FoundActors);

	FleeBrainComponents.Init(nullptr, FoundActors.Num());
	for (int i = 0; i < FleeBrainComponents.Num(); i++)
	{
		UFleeBrainComponent* BrainComponent = FoundActors[i]->FindComponentByClass<UFleeBrainComponent>();
		if(BrainComponent == nullptr) continue;
		
		BrainComponent->InitBrain(i, Settings->FollowFleeDetectionRadius, Settings->FleeDetectionRadius);
		FleeBrainComponents[i] = BrainComponent;
		
		BrainComponent->LeaderCortexStartFleeEvent.AddDynamic(this, &UFleeSubsystem::OnLeaderStartFlee);
		BrainComponent->LeaderCortexStopFleeEvent.AddDynamic(this, &UFleeSubsystem::OnLeaderStopFlee);
	}
}

void UFleeSubsystem::UpdateSubsystem(float InDeltaTime)
{
	for (TTuple<int, FFleeGroupData>& Pair : ActiveFleeGroups)
	{
		Pair.Value.GroupDirection = FleeBrainComponents[Pair.Key]->GetOwner()->GetActorForwardVector();
		
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
	const FFleeGroupData Data = {FleeBrainComponents[LeaderIndex]->GetFleeLeaderComponent()};
	ActiveFleeGroups.Add(LeaderIndex,Data);
	PropagateFlee(LeaderIndex);
}

void UFleeSubsystem::OnLeaderStopFlee(int LeaderIndex)
{
	FFleeGroupData* Data = ActiveFleeGroups.Find(LeaderIndex);
	for (UFleeFollowerComponent* Follower : Data->Followers)
	{
		Follower->RemoveGroupFollowed(LeaderIndex);
	}
	
	ActiveFleeGroups.Remove(LeaderIndex);
}

void UFleeSubsystem::PropagateFlee(const int StartIndex)
{
	UFleeBrainComponent* BrainComponent = FleeBrainComponents[StartIndex];
	if(BrainComponent == nullptr) return;
	
	TQueue<UFleeBrainComponent*> Frontier;
	Frontier.Enqueue(BrainComponent);
	TArray<UFleeBrainComponent*> VisitedBrains;

	UFleeBrainComponent* CurrentBrain = nullptr;
	while(!Frontier.IsEmpty())
	{
		Frontier.Dequeue(CurrentBrain);
		VisitedBrains.Add(CurrentBrain);

		for (UFleeBrainComponent* LinkedBrain : CurrentBrain->GetLinkedBrainComponents())
		{
			if(VisitedBrains.Contains(LinkedBrain)) continue;
			
			Frontier.Enqueue(LinkedBrain);
		}

		if(CurrentBrain->GetBrainIndex() == StartIndex) continue;

		UFleeFollowerComponent* FollowerComponent = CurrentBrain->GetFleeFollowerComponent();
		if(FollowerComponent->FollowsGroup(StartIndex)) continue;

		AddFollowerToGroup(StartIndex, FollowerComponent);
	}
}
#pragma endregion 

#pragma region FleeFollowers
void UFleeSubsystem::AddFollowerToGroup(const int InGroupLeaderIndex, UFleeFollowerComponent* InFollowerToAdd)
{
	if(!ActiveFleeGroups.Contains(InGroupLeaderIndex)) return;

	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	Data->Followers.AddUnique(InFollowerToAdd);

	InFollowerToAdd->AddGroupFollowed(InGroupLeaderIndex);
}

void UFleeSubsystem::RemoveFollowerFromGroup(const int InGroupLeaderIndex, UFleeFollowerComponent* InFollowerToRemove)
{
	if(!ActiveFleeGroups.Contains(InGroupLeaderIndex)) return;

	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	Data->Followers.Remove(InFollowerToRemove);

	InFollowerToRemove->RemoveGroupFollowed(InGroupLeaderIndex);
}
#pragma endregion 
