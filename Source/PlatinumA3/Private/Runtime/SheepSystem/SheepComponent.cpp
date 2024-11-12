// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/SheepComponent.h"

#pragma region Unreal Defaults

// Sets default values for this component's properties
USheepComponent::USheepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USheepComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USheepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




#pragma endregion 
#pragma region Sheep

// void USheepComponent::CaptureSheep()
// {
// 	bIsCaptured = true;
//
// 	GEngine->AddOnScreenDebugMessage(
// 		-1,
// 		4.0f,
// 		FColor::Red,
// 		TEXT("Sheep Captured !!"));
// }

bool USheepComponent::GetIsCaptured() const
{
	return bIsCaptured;	
}
void USheepComponent::SetIsCaptured(bool IsCaptured)
{
	bIsCaptured = IsCaptured;
}


#pragma endregion 
