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
		
		FFleeBrainInitData BrainInitData =
			{i,
			Settings->FollowFleeDetectionRadius,
			Settings->FleeDetectionRadius,
			Settings->PostFleeTime};
		
		BrainComponent->InitBrain(BrainInitData);
		FleeBrainComponents[i] = BrainComponent;

		//Bind to Brain Events
		BrainComponent->LinkEvent.AddDynamic(this, &UFleeSubsystem::OnLinkEvent);
		BrainComponent->UnlinkEvent.AddDynamic(this, &UFleeSubsystem::OnUnlinkEvent);
		
		//Bind to LeaderCortex Events
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
			if(Data == nullptr) continue;
			Data->GroupDirection = Pair.Value.GroupDirection;
		}
	}
}
#pragma region FleeBrains
void UFleeSubsystem::OnLinkEvent(UFleeBrainComponent* Brain, UFleeBrainComponent* OtherBrain)
{
	UFleeLeaderComponent* LeaderComponent = Brain->GetFleeLeaderComponent();
	UFleeFollowerComponent* FollowerComponent = Brain->GetFleeFollowerComponent();
	
	const UFleeLeaderComponent* OtherLeaderComponent = OtherBrain->GetFleeLeaderComponent();
	UFleeFollowerComponent* OtherFollowerComponent = OtherBrain->GetFleeFollowerComponent();

	if(LeaderComponent->GetFleeing()
		&& !OtherFollowerComponent->FollowsGroup(LeaderComponent->GetLeaderIndex()))
	{
		// AddFollowerToGroup(OtherBrain->GetBrainIndex(), FollowerComponent);
		PropagateFlee(OtherFollowerComponent->GetFollowerIndex(), LeaderComponent->GetLeaderIndex());
	}
	
	if(OtherLeaderComponent->GetFleeing()
		&& !FollowerComponent->FollowsGroup(OtherLeaderComponent->GetLeaderIndex()))
	{
		// AddFollowerToGroup(OtherBrain->GetBrainIndex(), FollowerComponent);
		PropagateFlee(FollowerComponent->GetFollowerIndex(), OtherLeaderComponent->GetLeaderIndex());
	}
	
	for (const TTuple<int, FGroupFollowedData> Pair : FollowerComponent->GetGroupFollowedDatas())
	{
		if(OtherFollowerComponent->FollowsGroup(Pair.Key)) continue;

		// AddFollowerToGroup(Pair.Key, FollowerComponent);
		PropagateFlee(OtherFollowerComponent->GetFollowerIndex(), Pair.Key);
	}
	
	for (const TTuple<int, FGroupFollowedData> Pair : OtherFollowerComponent->GetGroupFollowedDatas())
	{
		if(FollowerComponent->FollowsGroup(Pair.Key)) continue;

		// AddFollowerToGroup(Pair.Key, FollowerComponent);
		PropagateFlee(FollowerComponent->GetFollowerIndex(), Pair.Key);
	}

	
}

void UFleeSubsystem::OnUnlinkEvent(UFleeBrainComponent* Brain, UFleeBrainComponent* OtherBrain)
{
	UFleeFollowerComponent* FollowerComponent = Brain->GetFleeFollowerComponent();

	UFleeFollowerComponent* OtherFollowerComponent = OtherBrain->GetFleeFollowerComponent();

	TArray<UFleeBrainComponent*> Visited;
	TArray<int> IndexesToRemove;
	
	for (const TTuple<int, FGroupFollowedData> Pair : FollowerComponent->GetGroupFollowedDatas())
	{
		if(FindPathToLeader(Brain, Pair.Key, Visited)) continue;
		IndexesToRemove.Add(Pair.Key);
		for (const UFleeBrainComponent* FleeBrainComponent : Visited)
		{
			if(FleeBrainComponent == Brain) continue;
			RemoveFollowerFromGroup(Pair.Key, FleeBrainComponent->GetFleeFollowerComponent());
			FleeBrainComponent->GetFleeFollowerComponent()->RemoveGroupFollowed(Pair.Key);
		}
	}
	
	for (const int Index : IndexesToRemove)
	{
		RemoveFollowerFromGroup(Index, FollowerComponent);
		FollowerComponent->RemoveGroupFollowed(Index);
	}
	IndexesToRemove.Empty();

	
	for (const TTuple<int, FGroupFollowedData> Pair : OtherFollowerComponent->GetGroupFollowedDatas())
	{
		if(FindPathToLeader(OtherBrain, Pair.Key, Visited)) continue;
		IndexesToRemove.Add(Pair.Key);
		for (const UFleeBrainComponent* FleeBrainComponent : Visited)
		{
			if(FleeBrainComponent == OtherBrain) continue;
			RemoveFollowerFromGroup(Pair.Key, FleeBrainComponent->GetFleeFollowerComponent());
			FleeBrainComponent->GetFleeFollowerComponent()->RemoveGroupFollowed(Pair.Key);
		}
	}
	
	for (const int Index : IndexesToRemove)
	{
		RemoveFollowerFromGroup(Index, OtherFollowerComponent);
		OtherFollowerComponent->RemoveGroupFollowed(Index);
	}
}

#pragma endregion 

#pragma region FleeLeaders
void UFleeSubsystem::OnLeaderStartFlee(int LeaderIndex)
{
	const FFleeGroupData Data = {FleeBrainComponents[LeaderIndex]->GetFleeLeaderComponent()};
	ActiveFleeGroups.Add(LeaderIndex,Data);
	PropagateFlee(LeaderIndex, LeaderIndex);
}

void UFleeSubsystem::OnLeaderStopFlee(int LeaderIndex)
{
	// FFleeGroupData* Data = ActiveFleeGroups.Find(LeaderIndex);
	// for (UFleeFollowerComponent* Follower : Data->Followers)
	// {
	// 	RemoveFollowerFromGroup(LeaderIndex, Follower);
	// 	// Follower->RemoveGroupFollowed(LeaderIndex);
	// }
	// ClearFollowerGroup(LeaderIndex);

	PropagateStopFlee(LeaderIndex, LeaderIndex);
	ActiveFleeGroups.Remove(LeaderIndex);
}

void UFleeSubsystem::PropagateFlee(const int StartIndex, const int LeaderIndex)
{
	UFleeBrainComponent* BrainComponent = FleeBrainComponents[StartIndex];
	if(BrainComponent == nullptr) return;
	
	TQueue<UFleeBrainComponent*> Frontier;
	Frontier.Enqueue(BrainComponent);
	TArray<UFleeBrainComponent*> VisitedBrains;
	VisitedBrains.Add(BrainComponent);

	UFleeBrainComponent* CurrentBrain = nullptr;
	while(!Frontier.IsEmpty())
	{
		Frontier.Dequeue(CurrentBrain);

		for (UFleeBrainComponent* LinkedBrain : CurrentBrain->GetLinkedBrainComponents())
		{
			if(VisitedBrains.Contains(LinkedBrain)) continue;

			VisitedBrains.Add(LinkedBrain);

			if(LinkedBrain->GetFleeFollowerComponent() == nullptr) continue;
			
			Frontier.Enqueue(LinkedBrain);
		}

		if(CurrentBrain->GetBrainIndex() == LeaderIndex) continue;

		UFleeFollowerComponent* FollowerComponent = CurrentBrain->GetFleeFollowerComponent();
		if(FollowerComponent->FollowsGroup(LeaderIndex)) continue;
		
		AddFollowerToGroup(LeaderIndex, FollowerComponent);
	}
}

void UFleeSubsystem::PropagateStopFlee(const int StartIndex, const int LeaderIndex)
{
	UFleeBrainComponent* BrainComponent = FleeBrainComponents[StartIndex];
	if(BrainComponent == nullptr) return;
	
	TQueue<UFleeBrainComponent*> Frontier;
	Frontier.Enqueue(BrainComponent);
	TArray<UFleeBrainComponent*> VisitedBrains;
	VisitedBrains.Add(BrainComponent);

	UFleeBrainComponent* CurrentBrain = nullptr;
	while(!Frontier.IsEmpty())
	{
		Frontier.Dequeue(CurrentBrain);

		for (UFleeBrainComponent* LinkedBrain : CurrentBrain->GetLinkedBrainComponents())
		{
			if(VisitedBrains.Contains(LinkedBrain)) continue;

			VisitedBrains.Add(LinkedBrain);

			if(LinkedBrain->GetFleeFollowerComponent() == nullptr) continue;
			
			Frontier.Enqueue(LinkedBrain);
		}

		if(CurrentBrain->GetBrainIndex() == LeaderIndex) continue;

		UFleeFollowerComponent* FollowerComponent = CurrentBrain->GetFleeFollowerComponent();
		if(!FollowerComponent->FollowsGroup(StartIndex)) continue;

		RemoveFollowerFromGroup(LeaderIndex, FollowerComponent);
		FollowerComponent->RemoveGroupFollowed(LeaderIndex);
	}
}
#pragma endregion 

#pragma region FleeFollowers
void UFleeSubsystem::AddFollowerToGroup(const int InGroupLeaderIndex, UFleeFollowerComponent* InFollowerToAdd)
{
	if(!ActiveFleeGroups.Contains(InGroupLeaderIndex)) return;
	// if(InFollowerToAdd->GetFollowerIndex() == InGroupLeaderIndex) return;
	
	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	Data->Followers.AddUnique(InFollowerToAdd);

	InFollowerToAdd->AddGroupFollowed(InGroupLeaderIndex);
}

void UFleeSubsystem::RemoveFollowerFromGroup(const int InGroupLeaderIndex, UFleeFollowerComponent* InFollowerToRemove)
{
	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);
	if(Data == nullptr) return;

	if(Data->Followers.Contains(InFollowerToRemove))
	{
		Data->Followers.Remove(InFollowerToRemove);

		InFollowerToRemove->RemoveGroupFollowed(InGroupLeaderIndex);
	}
}

void UFleeSubsystem::ClearFollowerGroup(const int InGroupLeaderIndex)
{
	if(!ActiveFleeGroups.Contains(InGroupLeaderIndex)) return;

	FFleeGroupData* Data = ActiveFleeGroups.Find(InGroupLeaderIndex);

	for (UFleeFollowerComponent* Follower : Data->Followers)
	{
		Follower->RemoveGroupFollowed(InGroupLeaderIndex);
	}

	Data->Followers.Empty();
}

bool UFleeSubsystem::FindPathToLeader(UFleeBrainComponent* InStart, int InGroupLeaderIndex, TArray<UFleeBrainComponent*>& InOutVisitedBrains)
{
	if(InStart == nullptr) return false;

	if(InStart->GetBrainIndex() == InGroupLeaderIndex) return true;

	InOutVisitedBrains.Empty();
	
	TQueue<UFleeBrainComponent*> Frontier;
	Frontier.Enqueue(InStart);
	InOutVisitedBrains.Add(InStart);

	UFleeBrainComponent* CurrentBrain = nullptr;
	while (!Frontier.IsEmpty())
	{
		Frontier.Dequeue(CurrentBrain);

		for (UFleeBrainComponent* LinkedBrain : CurrentBrain->GetLinkedBrainComponents())
		{
			if(InOutVisitedBrains.Contains(LinkedBrain)) continue;

			InOutVisitedBrains.Add(LinkedBrain);

			if(LinkedBrain->GetFleeFollowerComponent() == nullptr) continue;

			if(LinkedBrain->GetBrainIndex() == InGroupLeaderIndex) return true;
			
			Frontier.Enqueue(LinkedBrain);
		}
	}
	return false;
}
#pragma endregion 
