// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeLeaderComponent.h"

#include "Runtime/FleeSystem/FleeSystemSettings.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
UFleeLeaderComponent::UFleeLeaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UFleeLeaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OnComponentBeginOverlap.AddDynamic(this, &UFleeLeaderComponent::OnDetectionBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UFleeLeaderComponent::OnDetectionEndOverlap);

	
	const UFleeSystemSettings* Settings = GetDefault<UFleeSystemSettings>();
	if (Settings == nullptr) return;

	TagToFleeFrom = Settings->TagToFleeFrom;
	// SphereRadius = Settings->FleeDetectionRadius;
	
}

void UFleeLeaderComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnComponentBeginOverlap.RemoveDynamic(this, &UFleeLeaderComponent::OnDetectionBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UFleeLeaderComponent::OnDetectionEndOverlap);
}


// Called every frame
void UFleeLeaderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(FleeLeadState == EFleeLeadState::Fleeing)
	{
		FVector NewFleeDirection;
		for (const AActor* ActorToFleeFrom : ScaryActors)
		{
			FVector AtoP = GetOwner()->GetActorLocation() - ActorToFleeFrom->GetActorLocation();
			NewFleeDirection += AtoP;
		}
		NewFleeDirection.Z = 0;
		NewFleeDirection.Normalize();
		FleeDirection = NewFleeDirection;
	}
	
	if(FleeLeadState == EFleeLeadState::PostFleeing)
	{
		PostFleeTimer -= DeltaTime;
		
		if(PostFleeTimer <= 0) StopFlee();

		// GEngine->AddOnScreenDebugMessage(
		// -1,
		// 4.0f,
		// FColor::Purple,
		// TEXT(" POST FLEE"));
	}
	// ...
}
#pragma endregion 
#pragma region FleeLeader

void UFleeLeaderComponent::Init(const unsigned InIndex, const float InDetectionRadius, const float InPostFleeTime)
{
	LeaderIndex = InIndex;
	SetSphereRadius(InDetectionRadius);
	// SphereRadius = InDetectionRadius;
	PostFleeTime = InPostFleeTime;
}

const TArray<AActor*>& UFleeLeaderComponent::GetScaryActors() const
{
	return ScaryActors;
}


bool UFleeLeaderComponent::GetFleeing() const
{
	return bFleeing;
}

EFleeLeadState UFleeLeaderComponent::GetFleeLeadState() const
{
	return FleeLeadState;
}

const FVector& UFleeLeaderComponent::GetFleeDirection() const
{
	return FleeDirection;
}

int UFleeLeaderComponent::GetLeaderIndex() const
{
	return LeaderIndex;
}

void UFleeLeaderComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(TagToFleeFrom))
	{
		ScaryActors.Add(OtherActor);
		if (ScaryActors.Num() == 1)
		{
			StartFlee();
		}
	}
}

void UFleeLeaderComponent::OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	if(Actor->ActorHasTag(TagToFleeFrom))
	{
		ScaryActors.Remove(Actor);
		if (ScaryActors.Num() == 0)
		{
			PostFlee();
			//StopFlee();
		}
	}
}

void UFleeLeaderComponent::StartFlee()
{
	bFleeing = true;
	FleeLeadState = EFleeLeadState::Fleeing;
	StartFleeEvent.Broadcast(LeaderIndex);
	JuicyStartFlee();
}

void UFleeLeaderComponent::PostFlee()
{
	PostFleeTimer = PostFleeTime;
	FleeLeadState = EFleeLeadState::PostFleeing;
	PostFleeEvent.Broadcast(LeaderIndex);
	JuicyPostFlee();
}


void UFleeLeaderComponent::StopFlee()
{
	bFleeing = false;
	FleeLeadState = EFleeLeadState::NotFleeing;
	StopFleeEvent.Broadcast(LeaderIndex);
	JuicyStopFlee();
}


#pragma endregion 
