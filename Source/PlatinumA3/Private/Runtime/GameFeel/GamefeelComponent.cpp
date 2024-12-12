// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/GameFeel/GamefeelComponent.h"

#pragma region UnrealDefaults
// Sets default values for this component's properties
UGamefeelComponent::UGamefeelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGamefeelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGamefeelComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGamefeelComponent::InitGamefeelComponent_Implementation(USceneComponent* InComponentToModify)
{
	ModifiedSceneComponent = InComponentToModify;
}
#pragma endregion 
