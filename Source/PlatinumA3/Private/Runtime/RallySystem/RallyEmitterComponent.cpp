// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/RallyEmitterComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/RallySystem/RallyReceiverComponent.h"
#include "Runtime/RallySystem/RallySystemSettings.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
URallyEmitterComponent::URallyEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URallyEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	const URallySystemSettings* Settings = GetDefault<URallySystemSettings>();
	if(Settings == nullptr) return;

	RallyEmitCooldown = Settings->RallyEmitCooldown;
}


// Called every frame
void URallyEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(RallyTimer > 0.0f)
	{
		RallyTimer -= DeltaTime;
	}
	
	// ...
}
#pragma endregion

#pragma region EmitSignal
void URallyEmitterComponent::EmitSignal()
{
	if(RallyTimer > 0.0f) return;
	
	RallyTimer = RallyEmitCooldown;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Real Rally"));
	const FVector Location = GetOwner()->GetActorLocation();

	const URallySystemSettings* RallySystemSettings = GetDefault<URallySystemSettings>();
	if(RallySystemSettings == nullptr) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	
	TArray<AActor*> FoundActors;
	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, RallySystemSettings->SignalRadius,
		ObjectTypes, AAIGroupCharacter::StaticClass(), ActorsToIgnore,FoundActors);

	for (const AActor* FoundActor : FoundActors)
	{
		URallyReceiverComponent* RallyReceiverComponent = FoundActor->FindComponentByClass<URallyReceiverComponent>();
		if(RallyReceiverComponent == nullptr) continue;

		RallyReceiverComponent->Notify(Location);
	}

	//Juicy Blueprint Event
	JuicyEmitSignal();
}

#pragma endregion 
