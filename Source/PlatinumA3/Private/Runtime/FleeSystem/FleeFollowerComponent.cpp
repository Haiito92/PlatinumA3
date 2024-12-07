// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeFollowerComponent.h"

#include "Components/SphereComponent.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
UFleeFollowerComponent::UFleeFollowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFleeFollowerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	DetectionCollision = Cast<USphereComponent>(
		GetOwner()->FindComponentByTag(USphereComponent::StaticClass(), "FleeFollower"));

	if(DetectionCollision==nullptr)return;

	DetectionCollision->OnComponentBeginOverlap.AddDynamic(this, &UFleeFollowerComponent::OnDetectionBeginOverlap);
	DetectionCollision->OnComponentEndOverlap.AddDynamic(this, &UFleeFollowerComponent::OnDetectionEndOverlap);
	
}

void UFleeFollowerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	DetectionCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UFleeFollowerComponent::OnDetectionBeginOverlap);
	DetectionCollision->OnComponentEndOverlap.RemoveDynamic(this, &UFleeFollowerComponent::OnDetectionEndOverlap);

}


// Called every frame
void UFleeFollowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region FleeFollower
void UFleeFollowerComponent::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;
	
	UFleeFollowerComponent* FleeFollowerComponent = OtherActor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;
	
	NeighbouringFollowers.Add(FleeFollowerComponent);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Green,
	// 	TEXT("Overlap Begin"));
}

void UFleeFollowerComponent::OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I)
{
	UFleeFollowerComponent* FleeFollowerComponent = Actor->FindComponentByClass<UFleeFollowerComponent>();
	if(FleeFollowerComponent == nullptr) return;

	NeighbouringFollowers.Remove(FleeFollowerComponent);
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Green,
	// 	TEXT("Overlap End"));
}
#pragma endregion 
