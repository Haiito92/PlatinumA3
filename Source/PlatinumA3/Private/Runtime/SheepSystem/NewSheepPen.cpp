// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/NewSheepPen.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Runtime/SheepSystem/SheepComponent.h"
#include "Runtime/SheepSystem/SheepSubsystem.h"

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
	PenEntryCollision->SetupAttachment(RootComponent);

	PenInsideSphere = CreateDefaultSubobject<USphereComponent>("PenInsideSphere");
	PenInsideSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANewSheepPen::BeginPlay()
{
	Super::BeginPlay();

	PenEntryCollision->OnComponentBeginOverlap.AddDynamic(this, &ANewSheepPen::OnEntryCollisionBeginOverlap);
	PenEntryCollision->OnComponentEndOverlap.AddDynamic(this, &ANewSheepPen::OnEntryCollisionEndOverlap);

	SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
}

// Called every frame
void ANewSheepPen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRoamingTimers(DeltaTime);
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

void ANewSheepPen::CaptureSheep(USheepComponent* SheepComponent)
{
	SheepComponent->Capture();
	
	SheepSubsystem->AddCapturedSheep(1);

	CapturedSheeps.Add(SheepComponent, 0.0f);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Magenta,
		TEXT("SHEEP CAPTURED !!"));
}


#pragma endregion 
#pragma region CapturedSheeps
void ANewSheepPen::UpdateRoamingTimers(float DeltaTime)
{
	for (TTuple<USheepComponent*, float>& SheepTimerPair : CapturedSheeps)
	{
		SheepTimerPair.Value -= DeltaTime;
		float Distance = FVector::Distance(SheepTimerPair.Key->GetOwner()->GetActorLocation(),
			SheepTimerPair.Key->GetCapturedRoamingLocation());
		
		if(SheepTimerPair.Value <= 0.0f || Distance <= 50.f)
		{
			SheepTimerPair.Value = 3.0f;
			GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Magenta,
		TEXT("UDPATE SHEEP TIMER"));
			//Calculate next roaming location
			GiveSheepNewRoamingLocation(SheepTimerPair.Key);
			
		}
	}
}

void ANewSheepPen::GiveSheepNewRoamingLocation(USheepComponent* SheepComponent) const
{
	if(PenInsideSphere == nullptr) return;
	
	const float InsidePenRadius = PenInsideSphere->GetScaledSphereRadius();
	if(InsidePenRadius <= 0.0f) return;
	const FVector Center = PenInsideSphere->GetComponentLocation();

	FVector NewLocation = FVector::ZeroVector;
	
	NewLocation.X = Center.X
	+ FMath::FRandRange(-InsidePenRadius, InsidePenRadius);

	NewLocation.Y = Center.Y
	+ FMath::FRandRange(-InsidePenRadius, InsidePenRadius);

	NewLocation.Z = Center.Z;
	
	SheepComponent->SetCapturedRoamingLocation(NewLocation);
}
#pragma endregion 
