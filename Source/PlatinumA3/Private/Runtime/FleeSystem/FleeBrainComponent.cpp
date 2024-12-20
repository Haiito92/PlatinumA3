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

void UFleeBrainComponent::InitBrain(const FFleeBrainInitData& InBrainInitData)
{
	BrainIndex = InBrainInitData.Index;
	SetSphereRadius(InBrainInitData.LinkRadius);

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AAIGroupCharacter::StaticClass());
	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if(OverlappingActor == GetOwner()) continue;
		UFleeBrainComponent* FleeBrainComponent = OverlappingActor->FindComponentByClass<UFleeBrainComponent>();
		if(FleeBrainComponent==nullptr) continue;

		LinkedBrainComponents.AddUnique(FleeBrainComponent);
	}

	InitLeaderCortex(InBrainInitData.Index, InBrainInitData.DetectionRadius, InBrainInitData.PostFleeTime);

	InitFollowerCortex(InBrainInitData.Index);
}

void UFleeBrainComponent::InitLeaderCortex(const int InIndex, const float InDetectionRadius, const float InPostFleeTime)
{
	FleeLeaderComponent = GetOwner()->FindComponentByClass<UFleeLeaderComponent>();
	if(FleeLeaderComponent != nullptr) FleeLeaderComponent->Init(InIndex, InDetectionRadius, InPostFleeTime);

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

	UFleeBrainComponent* OtherBrainComponent = OtherActor->FindComponentByClass<UFleeBrainComponent>();
	if(OtherBrainComponent == nullptr) return;
	if(LinkedBrainComponents.Contains(OtherBrainComponent)) return;
	
	LinkedBrainComponents.AddUnique(OtherBrainComponent);
	OtherBrainComponent->GetLinkedBrainComponents().AddUnique(this);

	LinkEvent.Broadcast(this, OtherBrainComponent);
}

void UFleeBrainComponent::OnLinkDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == GetOwner()) return;

	UFleeBrainComponent* OtherBrainComponent = OtherActor->FindComponentByClass<UFleeBrainComponent>();
	if(OtherBrainComponent == nullptr) return;
	if(!LinkedBrainComponents.Contains(OtherBrainComponent)) return;
	
	LinkedBrainComponents.Remove(OtherBrainComponent);
	OtherBrainComponent->GetLinkedBrainComponents().Remove(this);
	
	UnlinkEvent.Broadcast(this, OtherBrainComponent);
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