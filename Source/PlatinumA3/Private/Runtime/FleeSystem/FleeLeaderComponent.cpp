// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeLeaderComponent.h"

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
	
}


// Called every frame
void UFleeLeaderComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#pragma endregion

#pragma region FleeLeader

// FVector UFleeLeaderComponent::GetLeaderLocation() const
// {
// 	return GetOwner()->GetActorLocation();
// }
//
// FVector UFleeLeaderComponent::GetLeaderForwardVector() const
// {
// 	return GetOwner()->GetActorForwardVector();
// }

#pragma endregion 
