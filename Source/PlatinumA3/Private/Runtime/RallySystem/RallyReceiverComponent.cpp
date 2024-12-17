// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/RallyReceiverComponent.h"

#include "Runtime/RallySystem/RallySystemSettings.h"
#include "Runtime/Utilities/BlueprintLibraries/DevSettingsBlueprintLibrary.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
URallyReceiverComponent::URallyReceiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URallyReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	const URallySystemSettings* Settings = UDevSettingsBlueprintLibrary::GetRallySystemSettings();
	if(Settings == nullptr) return;

	RallySpeed = Settings->RallySpeed;
}


// Called every frame
void URallyReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(bIsNotified)
	{
		if(FVector::Distance(Destination, GetOwner()->GetActorLocation()) <= RallySpeed*DeltaTime)
		{
			UnNotify();
		}
	}
}
#pragma endregion 

bool URallyReceiverComponent::GetIsNotified() const
{
	return bIsNotified;
}

const FVector& URallyReceiverComponent::GetDestination() const
{
	return Destination;
}

void URallyReceiverComponent::Notify(const FVector& NewDestination)
{
	const URallySystemSettings* RallySystemSettings = GetDefault<URallySystemSettings>();
	if(RallySystemSettings == nullptr) return;
	
	bIsNotified = true;
	Destination = NewDestination;
	GetWorld()->GetTimerManager().SetTimer(
		IsNotifiedTimerHandle,
		this,
		&URallyReceiverComponent::OnNotifyTimeOut,
		RallySystemSettings->RallyTime,
		false);

	//Juicy
	JuicyNotify();
}

void URallyReceiverComponent::UnNotify()
{
	bIsNotified = false;
	IsNotifiedTimerHandle.Invalidate();

	//Juicy
	JuicyUnNotify();
}

void URallyReceiverComponent::OnNotifyTimeOut()
{
	UnNotify();
}

