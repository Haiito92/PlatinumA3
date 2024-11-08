// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/RallyReceiverComponent.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
URallyReceiverComponent::URallyReceiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URallyReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URallyReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion 

bool URallyReceiverComponent::GetIsNotified() const
{
	return bIsNotified;
}

const FVector& URallyReceiverComponent::GetDestination() const
{
	return Destination;
}

void URallyReceiverComponent::Notify(const FVector& NewDestination)
{
	bIsNotified = true;
	Destination = NewDestination;
	GetWorld()->GetTimerManager().SetTimer(
		IsNotifiedTimerHandle,
		this,
		&URallyReceiverComponent::OnNotifyTimeOut,
		3.0f,
		false);
}

void URallyReceiverComponent::OnNotifyTimeOut()
{
	bIsNotified = false;
	IsNotifiedTimerHandle.Invalidate();
}

