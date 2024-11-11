// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/RallyEmitterComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/RallySystem/RallyReceiverComponent.h"
#include "Runtime/RallySystem/RallySystemSettings.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
URallyEmitterComponent::URallyEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URallyEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URallyEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region EmitSignal
void URallyEmitterComponent::EmitSignal() const
{
	const FVector Location = GetOwner()->GetActorLocation();

	const URallySystemSettings* RallySystemSettings = GetDefault<URallySystemSettings>();
	if(RallySystemSettings == nullptr) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	
	TArray<AActor*> FoundActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, RallySystemSettings->SignalRadius,
		ObjectTypes, AAIGroupPawn::StaticClass(), ActorsToIgnore,FoundActors);

	for (const AActor* FoundActor : FoundActors)
	{
		URallyReceiverComponent* RallyReceiverComponent = FoundActor->FindComponentByClass<URallyReceiverComponent>();
		if(RallyReceiverComponent == nullptr) continue;

		RallyReceiverComponent->Notify(Location);
	}
}

#pragma endregion 
