// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/GameFeel/WaddlingComponent.h"

#pragma region UnrealDefaults
// Sets default values for this component's properties
UWaddlingComponent::UWaddlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaddlingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWaddlingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#pragma endregion 

#pragma region Waddling
void UWaddlingComponent::StartWaddling_Implementation()
{
}
void UWaddlingComponent::StopWaddling_Implementation()
{
}
#pragma endregion 