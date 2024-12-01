// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeBrainComponent.h"

#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

#pragma region UnrealDefaults
// Sets default values for this component's properties
UFleeBrainComponent::UFleeBrainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFleeBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UFleeBrainComponent::OnLinkDetectionBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UFleeBrainComponent::OnLinkDetectionEndOverlap);
	// ...
}

void UFleeBrainComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnComponentBeginOverlap.RemoveDynamic(this, &UFleeBrainComponent::OnLinkDetectionBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UFleeBrainComponent::OnLinkDetectionEndOverlap);

	if(FleeLeaderComponent != nullptr)
	{
		FleeLeaderComponent->StartFleeEvent.RemoveDynamic(this, &UFleeBrainComponent::OnLeaderCortexStartFleeEvent);
		FleeLeaderComponent->StopFleeEvent.RemoveDynamic(this, &UFleeBrainComponent::OnLeaderCortexStopFleeEvent);
	}
}


// Called every frame
void UFleeBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

#pragma endregion
#pragma region Initialization

void UFleeBrainComponent::InitBrain(const int InIndex, const float InLinkRadius, const float InDetectionRadius)
{
	BrainIndex = InIndex;
	SetSphereRadius(InLinkRadius);

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AAIGroupCharacter::StaticClass());
	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if(OverlappingActor == GetOwner()) continue;
		UFleeBrainComponent* FleeBrainComponent = OverlappingActor->FindComponentByClass<UFleeBrainComponent>();
		if(FleeBrainComponent==nullptr) continue;

		LinkedBrainComponents.AddUnique(FleeBrainComponent);
	}

	InitLeaderCortex(InIndex, InDetectionRadius);

	InitFollowerCortex(InIndex);
}

void UFleeBrainComponent::InitLeaderCortex(const int InIndex, const float InDetectionRadius)
{
	FleeLeaderComponent = GetOwner()->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr) FleeLeaderComponent->Init(InIndex, InDetectionRadius);

	FleeLeaderComponent->StartFleeEvent.AddDynamic(this, &UFleeBrainComponent::OnLeaderCortexStartFleeEvent);
	FleeLeaderComponent->StopFleeEvent.AddDynamic(this, &UFleeBrainComponent::OnLeaderCortexStopFleeEvent);
}

void UFleeBrainComponent::InitFollowerCortex(const int InIndex)
{
	FleeFollowerComponent = GetOwner()->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent != nullptr) FleeFollowerComponent->Init(InIndex);
}

int UFleeBrainComponent::GetBrainIndex() const
{
	return BrainIndex;
}

#pragma endregion 
#pragma region Brain&Links


TArray<UFleeBrainComponent*>& UFleeBrainComponent::GetLinkedBrainComponents()
{
	return LinkedBrainComponents;
}

void UFleeBrainComponent::OnLinkDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;

	UFleeBrainComponent* FleeBrainComponent = OtherActor->FindComponentByClass<UFleeBrainComponent>();
	if(FleeBrainComponent == nullptr) return;

	LinkedBrainComponents.AddUnique(FleeBrainComponent);
}

void UFleeBrainComponent::OnLinkDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == GetOwner()) return;

	UFleeBrainComponent* FleeBrainComponent = OtherActor->FindComponentByClass<UFleeBrainComponent>();
	if(FleeBrainComponent == nullptr) return;

	LinkedBrainComponents.Remove(FleeBrainComponent);
}
#pragma endregion

#pragma region LeaderCortex

UFleeLeaderComponent* UFleeBrainComponent::GetFleeLeaderComponent() const
{
	return FleeLeaderComponent;
}

void UFleeBrainComponent::OnLeaderCortexStartFleeEvent(int InLeaderIndex)
{
	LeaderCortexStartFleeEvent.Broadcast(InLeaderIndex);
}

void UFleeBrainComponent::OnLeaderCortexStopFleeEvent(int InLeaderIndex)
{
	LeaderCortexStopFleeEvent.Broadcast(InLeaderIndex);
}

#pragma endregion 

#pragma region FollowerCortex
UFleeFollowerComponent* UFleeBrainComponent::GetFleeFollowerComponent() const
{
	return FleeFollowerComponent;
}
#pragma endregion 