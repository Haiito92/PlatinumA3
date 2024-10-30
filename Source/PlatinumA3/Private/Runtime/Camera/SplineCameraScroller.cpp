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

void ASplineCameraScroller::InitializedSplineCameraScroller(TArray<AStateCharacter*> Characters)
{
	if(!Characters[0] || !Characters[1]) return;
	
	m_Berger = Cast<AWoolStateCharacter>(Characters[0]);
	m_Chien = Cast<AWoolStateCharacter>(Characters[1]);
	


	if(!m_CameraActor)
	{
		m_CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	}
}

void ASplineCameraScroller::UpdateSplineCamera()
{
	if(!m_Berger || !m_Chien || !m_CameraActor) return;
	
	FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);

	FVector CameraTargetLocation = m_SplineComponent->FindLocationClosestToWorldLocation(InterpolatedLocation, ESplineCoordinateSpace::World);

	FVector LerpedLocation = FMath::Lerp(m_CameraActor->GetActorLocation(), CameraTargetLocation, m_CameraMoveSpeed * GetWorld()->GetDeltaSeconds());
	
	m_CameraActor->SetActorLocation(LerpedLocation);

	LookAtTargetSmooth(InterpolatedLocation);
}

void ASplineCameraScroller::LookAtTargetSmooth(FVector TargetLocation)
{
	FVector MyLocation = m_CameraActor->GetActorLocation();
	FVector Direction = (TargetLocation - MyLocation).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	FRotator NewRotation = FMath::RInterpTo(m_CameraActor->GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), m_CameraRotationSpeed);
	
	m_CameraActor->SetActorRotation(NewRotation);
}

