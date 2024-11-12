// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Scarable/ScarableComponent.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
UScarableComponent::UScarableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScarableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UScarableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region IScarable

void UScarableComponent::Scare_Implementation(const AActor* ScaryActor)
{
	IScarable::Scare_Implementation(ScaryActor);

	ScareEvent.Broadcast(ScaryActor);
}

void UScarableComponent::UnScare_Implementation()
{
	IScarable::UnScare_Implementation();
	UnScareEvent.Broadcast();
}

void UScarableComponent::LowScare_Implementation(FVector LowFleeDirection, float SignalRadius)
{
	IScarable::LowScare_Implementation(LowFleeDirection, SignalRadius);
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Orange,
	// 	TEXT("Scarable Component LowScare"));
	LowScareEvent.Broadcast(LowFleeDirection,SignalRadius);
}

#pragma endregion 
