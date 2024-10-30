// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Camera/SplineCameraScroller.h"

#include "Camera/CameraActor.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Characters/WoolStateCharacter.h"


// Sets default values
ASplineCameraScroller::ASplineCameraScroller()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

// Called when the game starts or when spawned
void ASplineCameraScroller::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ASplineCameraScroller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSplineCamera();
}

void ASplineCameraScroller::InitializedSplineCameraScroller(AWoolStateCharacter* InBerger, AWoolStateCharacter* InChien)
{
	if(!InBerger || !InChien) return;
	
	m_Berger = InBerger;
	m_Chien = InChien;
	

	m_CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), TSubclassOf<ACameraActor>()));

	if(m_CameraActor)
	{
		
	}else
	{
		
	}
}

void ASplineCameraScroller::UpdateSplineCamera()
{
	if(!m_Berger || !m_Chien || !m_CameraActor) return;
	
	FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);

	FVector CameraTargetLocation = m_SplineComponent->FindLocationClosestToWorldLocation(InterpolatedLocation, ESplineCoordinateSpace::World);

	m_CameraActor->SetActorLocation(CameraTargetLocation);
}

