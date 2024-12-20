// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/SheepComponent.h"

#include "Runtime/SheepSystem/SheepPenSystemSettings.h"
#include "Runtime/SheepSystem/SheepStateFlag.h"
#include "Runtime/SheepSystem/SheepSubsystem.h"

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

void USheepComponent::InitSheep(int Index)
{
	SheepIndex = Index;
}

void USheepComponent::Birth()
{
	bIsAlive = true;
	bIsCaptured = false;
	SheepBirthEvent.Broadcast(SheepIndex);
	JuicyBirth();
}

void USheepComponent::Capture()
{
	bIsCaptured = true;

	JuicyCapture();
}

void USheepComponent::Die()
{
	bIsAlive = false;
	bIsCaptured = false;
	SheepDeathEvent.Broadcast(SheepIndex);

	JuicyDie();
}

int USheepComponent::GetSheepIndex() const
{
	return SheepIndex;
}
void USheepComponent::SetIndex(const int Index)
{
	SheepIndex = Index;
}

bool USheepComponent::GetIsAlive() const
{
	return bIsAlive;
}

void USheepComponent::SetIsAlive(const bool InIsAlive)
{
	bIsAlive = InIsAlive;
}

bool USheepComponent::GetIsCaptured() const
{
	return bIsCaptured;	
}
void USheepComponent::SetIsCaptured(const bool IsCaptured)
{
	bIsCaptured = IsCaptured;
}

const FVector& USheepComponent::GetCapturedRoamingLocation() const
{
	return CapturedRoamingLocation;
}
void USheepComponent::SetCapturedRoamingLocation(const FVector& NewValue)
{
	CapturedRoamingLocation = NewValue;
}
#pragma endregion 
