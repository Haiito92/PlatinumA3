// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeFollowerComponent.h"

#include "Components/SphereComponent.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"

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
	
	OnComponentBeginOverlap.AddDynamic(this, &UFleeFollowerComponent::OnDetectionBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UFleeFollowerComponent::OnDetectionEndOverlap);
	
}

void UFleeFollowerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	OnComponentBeginOverlap.RemoveDynamic(this, &UFleeFollowerComponent::OnDetectionBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UFleeFollowerComponent::OnDetectionEndOverlap);

}


// Called every frame
void UFleeFollowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region FleeFollower
TMap<int, int>& UFleeFollowerComponent::GetGroupFollowedIndexes()
{
	return GroupFollowedIndexes;
}

int UFleeFollowerComponent::GetFollowerIndex() const
{
	return FollowerIndex;
}

void UFleeFollowerComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;

	UFleeLeaderComponent* FleeLeaderComponent = OtherActor->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr)
	{
		const int LeaderIndex = FleeLeaderComponent->GetLeaderIndex();
		
		if(FleeLeaderComponent->GetFleeing())
		{
			IncrementGroupAmount(LeaderIndex);
		}
		
		NeighbouringLeaders.Add(FleeLeaderComponent);
		FleeLeaderComponent->StartFleeEvent.AddDynamic(this, &UFleeFollowerComponent::OnLeaderStartFleeEvent);
		FleeLeaderComponent->StopFleeEvent.AddDynamic(this, &UFleeFollowerComponent::OnLeaderStopFleeEvent);
	}
	
	
	UFleeFollowerComponent* FleeFollowerComponent = OtherActor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;
	
	NeighbouringFollowers.Add(FleeFollowerComponent);
	
}

void UFleeFollowerComponent::OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	UFleeLeaderComponent* FleeLeaderComponent = Actor->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr)
	{
		const int LeaderIndex = FleeLeaderComponent->GetLeaderIndex();
		if(FleeLeaderComponent->GetFleeing())
		{
			DecrementGroupAmount(LeaderIndex);
		}
		
		NeighbouringLeaders.Remove(FleeLeaderComponent);
		FleeLeaderComponent->StartFleeEvent.RemoveDynamic(this, &UFleeFollowerComponent::OnLeaderStartFleeEvent);
		FleeLeaderComponent->StopFleeEvent.RemoveDynamic(this, &UFleeFollowerComponent::OnLeaderStopFleeEvent);
	}
	
	UFleeFollowerComponent* FleeFollowerComponent = Actor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;

	NeighbouringFollowers.Remove(FleeFollowerComponent);
	
}

void UFleeFollowerComponent::OnLeaderStartFleeEvent(int LeaderIndex)
{
	IncrementGroupAmount(LeaderIndex);
}

void UFleeFollowerComponent::OnLeaderStopFleeEvent(int LeaderIndex)
{
	DecrementGroupAmount(LeaderIndex);
}

void UFleeFollowerComponent::IncrementGroupAmount(int InGroupLeaderIndex, int InAmount)
{
	if(!GroupFollowedIndexes.Contains(InGroupLeaderIndex))
	{
		GroupFollowedIndexes.Add(InGroupLeaderIndex, InAmount);
		EncounteredNewGroupEvent.Broadcast(InGroupLeaderIndex, FollowerIndex);
	}else
	{
		int* Amount = GroupFollowedIndexes.Find(InGroupLeaderIndex);
		(*Amount) += InAmount;
	}
}

void UFleeFollowerComponent::DecrementGroupAmount(int InGroupLeaderIndex, int InAmount)
{
	if(!GroupFollowedIndexes.Contains(InGroupLeaderIndex)) return;

	int* Amount = GroupFollowedIndexes.Find(InGroupLeaderIndex);
	(*Amount) -= InAmount;
	
	if((*Amount) <= 0)
	{
		LostContactWithGroupEvent.Broadcast(InGroupLeaderIndex, FollowerIndex);
	
		GroupFollowedIndexes.Remove(InGroupLeaderIndex);
	}
}
#pragma endregion 
