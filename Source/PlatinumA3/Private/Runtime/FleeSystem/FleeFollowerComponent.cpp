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
void UFleeFollowerComponent::Init(int InFollowerIndex)
{
	FollowerIndex = InFollowerIndex;
	const UFleeSystemSettings* Settings = GetDefault<UFleeSystemSettings>();
	if(Settings!= nullptr)
	{
		SphereRadius = Settings->FollowFleeDetectionRadius;
	}
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AAIGroupCharacter::StaticClass());
	
	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if(OverlappingActor == GetOwner()) continue;
		
		UFleeLeaderComponent* FleeLeaderComponent = OverlappingActor->FindComponentByClass<UFleeLeaderComponent>();
		if(FleeLeaderComponent != nullptr) AddNeighbouringLeader(FleeLeaderComponent);

		UFleeFollowerComponent* FleeFollowerComponent = OverlappingActor->FindComponentByClass<UFleeFollowerComponent>();
		if(FleeFollowerComponent != nullptr) AddNeighbouringFollower(FleeFollowerComponent);
	}
}

TMap<int, FGroupFollowedData>& UFleeFollowerComponent::GetGroupFollowedDatas()
{
	return GroupFollowedDatas;
}

int UFleeFollowerComponent::GetFollowerIndex() const
{
	return FollowerIndex;
}

bool UFleeFollowerComponent::GetFollowFleeing() const
{
	return bFollowFleeing;
}

void UFleeFollowerComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;
	
	UFleeLeaderComponent* FleeLeaderComponent = OtherActor->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr)
	{
		AddNeighbouringLeader(FleeLeaderComponent);
	}
	
	
	UFleeFollowerComponent* FleeFollowerComponent = OtherActor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;
	
	AddNeighbouringFollower(FleeFollowerComponent);	
}

void UFleeFollowerComponent::OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	UFleeLeaderComponent* FleeLeaderComponent = Actor->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr)
	{
		RemoveNeighbouringLeader(FleeLeaderComponent);
	}
	
	UFleeFollowerComponent* FleeFollowerComponent = Actor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;

	RemoveNeighbouringFollower(FleeFollowerComponent);
	
}

void UFleeFollowerComponent::OnLeaderStartFleeEvent(int LeaderIndex)
{
	IncrementGroupAmount(LeaderIndex);
}

void UFleeFollowerComponent::OnLeaderStopFleeEvent(int LeaderIndex)
{
	DecrementGroupAmount(LeaderIndex);
}

void UFleeFollowerComponent::AddNeighbouringLeader(UFleeLeaderComponent* LeaderComponent)
{
	const int LeaderIndex = LeaderComponent->GetLeaderIndex();
		
	if(LeaderComponent->GetFleeing())
	{
		IncrementGroupAmount(LeaderIndex);
	}
		
	NeighbouringLeaders.AddUnique(LeaderComponent);
	LeaderComponent->StartFleeEvent.AddDynamic(this, &UFleeFollowerComponent::OnLeaderStartFleeEvent);
	LeaderComponent->StopFleeEvent.AddDynamic(this, &UFleeFollowerComponent::OnLeaderStopFleeEvent);
}

void UFleeFollowerComponent::RemoveNeighbouringLeader(UFleeLeaderComponent* LeaderComponent)
{
	const int LeaderIndex = LeaderComponent->GetLeaderIndex();
	if(LeaderComponent->GetFleeing())
	{
		DecrementGroupAmount(LeaderIndex);
	}
		
	NeighbouringLeaders.Remove(LeaderComponent);
	LeaderComponent->StartFleeEvent.RemoveDynamic(this, &UFleeFollowerComponent::OnLeaderStartFleeEvent);
	LeaderComponent->StopFleeEvent.RemoveDynamic(this, &UFleeFollowerComponent::OnLeaderStopFleeEvent);
}

void UFleeFollowerComponent::AddNeighbouringFollower(UFleeFollowerComponent* FollowerComponent)
{
	NeighbouringFollowers.AddUnique(FollowerComponent);
}

void UFleeFollowerComponent::RemoveNeighbouringFollower(UFleeFollowerComponent* FollowerComponent)
{
	NeighbouringFollowers.Remove(FollowerComponent);
}

void UFleeFollowerComponent::IncrementGroupAmount(int InGroupLeaderIndex, int InAmount)
{
	if(!GroupFollowedDatas.Contains(InGroupLeaderIndex))
	{
		const FGroupFollowedData Data = {InAmount,FVector::ZeroVector};
		GroupFollowedDatas.Add(InGroupLeaderIndex, Data);
		EncounteredNewGroupEvent.Broadcast(InGroupLeaderIndex, FollowerIndex);

		if( GroupFollowedDatas.Num() == 1) StartFollowFlee();
	}else
	{
		FGroupFollowedData* Data = GroupFollowedDatas.Find(InGroupLeaderIndex);
		Data->NeighboursAmount += InAmount;
	}
}

void UFleeFollowerComponent::DecrementGroupAmount(int InGroupLeaderIndex, int InAmount)
{
	if(!GroupFollowedDatas.Contains(InGroupLeaderIndex)) return;

	FGroupFollowedData* Data = GroupFollowedDatas.Find(InGroupLeaderIndex);
	
	Data->NeighboursAmount -= InAmount;
	
	if(Data->NeighboursAmount <= 0)
	{
		LostContactWithGroupEvent.Broadcast(InGroupLeaderIndex, FollowerIndex);
	
		GroupFollowedDatas.Remove(InGroupLeaderIndex);

		if( GroupFollowedDatas.Num() == 0) StopFollowFlee();
	}
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
