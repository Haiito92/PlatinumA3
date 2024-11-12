// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/NewSheepPen.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Runtime/SheepSystem/SheepComponent.h"

#pragma region Unreal Defaults

// Sets default values
ANewSheepPen::ANewSheepPen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PenFenceSpline = CreateDefaultSubobject<USplineComponent>("PenFenceSpline");
	RootComponent = PenFenceSpline;
	
	// PenFence = CreateDefaultSubobject<USplineMeshComponent>("PenFence");
	
	PenEntryCollision = CreateDefaultSubobject<UBoxComponent>("PenEntryCollision");
	PenEntryCollision->SetupAttachment(PenFenceSpline);
}

// Called when the game starts or when spawned
void ANewSheepPen::BeginPlay()
{
	Super::BeginPlay();

	PenEntryCollision->OnComponentBeginOverlap.AddDynamic(this, &ANewSheepPen::OnEntryCollisionBeginOverlap);
	PenEntryCollision->OnComponentEndOverlap.AddDynamic(this, &ANewSheepPen::OnEntryCollisionEndOverlap);
	
}

// Called every frame
void ANewSheepPen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewSheepPen::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	PenEntryCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ANewSheepPen::OnEntryCollisionBeginOverlap);
	PenEntryCollision->OnComponentEndOverlap.RemoveDynamic(this, &ANewSheepPen::OnEntryCollisionEndOverlap);
}


#pragma endregion 

#pragma region SheepEntry

void ANewSheepPen::OnEntryCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USheepComponent* SheepComponent = OtherActor->FindComponentByClass<USheepComponent>();
	if(SheepComponent == nullptr) return;

	if(!SheepComponent->GetIsCaptured())
	{
		CaptureSheep(SheepComponent);
	}
	
	
}

void ANewSheepPen::OnEntryCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ANewSheepPen::CaptureSheep(USheepComponent* SheepComponent) const
{
	SheepComponent->SetIsCaptured(true);

	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Red,
		TEXT("SHEEP CAPTURED !!"));
}
#pragma endregion 
