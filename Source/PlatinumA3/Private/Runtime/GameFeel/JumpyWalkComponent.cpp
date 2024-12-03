// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/GameFeel/JumpyWalkComponent.h"

#pragma region UnrealDefaults
// Sets default values for this component's properties
UJumpyWalkComponent::UJumpyWalkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJumpyWalkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJumpyWalkComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region JumpyWalk
void UJumpyWalkComponent::StartJumpyWalk_Implementation()
{
	
}

void UJumpyWalkComponent::StopJumpyWalk_Implementation()
{
}
#pragma endregion 