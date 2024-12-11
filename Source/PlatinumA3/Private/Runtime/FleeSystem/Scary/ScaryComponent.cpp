// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/Scary/ScaryComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"
#include "Runtime/FleeSystem/Scary/IScarable.h"


// Sets default values for this component's properties
UScaryComponent::UScaryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScaryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OnComponentBeginOverlap.AddDynamic(this, &UScaryComponent::OnDetectionBeginOverlapEvent);
	OnComponentEndOverlap.AddDynamic(this, &UScaryComponent::OnDetectionEndOverlapEvent);
	
	const UFleeSystemSettings* Settings = GetDefault<UFleeSystemSettings>();
	if(Settings == nullptr) return;

	SetSphereRadius(Settings->FleeDetectionRadius);
	BarkMinCooldown = Settings->BarkCooldownMin;
	BarkMaxCooldown = Settings->BarkCooldownMax;
}

void UScaryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnComponentBeginOverlap.RemoveDynamic(this, &UScaryComponent::OnDetectionBeginOverlapEvent);
	OnComponentEndOverlap.RemoveDynamic(this, &UScaryComponent::OnDetectionEndOverlapEvent);
}


// Called every frame
void UScaryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// UE_LOGFMT(LogTemp, Warning, "Number : {0}", NbScarableInsideRadius);
	if(NbScarableInsideRadius <= 0) return;
	
	if(BarkTimer > 0)
	{
		BarkTimer -= DeltaTime;
	}

	if(BarkTimer <= 0)
	{
		Bark();
	}
}

void UScaryComponent::OnDetectionBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == GetOwner()) return;
	
	if(OtherActor->Implements<UScarable>()) NbScarableInsideRadius ++;
}

void UScaryComponent::OnDetectionEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == GetOwner()) return;
	
	if(OtherActor->Implements<UScarable>()) NbScarableInsideRadius --;
}

void UScaryComponent::Bark()
{
	BarkTimer = UKismetMathLibrary::RandomFloatInRange(BarkMinCooldown, BarkMaxCooldown);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Bark"));
	
	ReceiveBark();
	JuicyBark();
}

