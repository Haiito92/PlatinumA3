// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Camera/SplineCameraScroller.h"

#include "Camera/CameraActor.h"
#include "Components/CapsuleComponent.h"
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
	UpdateCharactersInFrustrum(m_Berger);
	UpdateCharactersInFrustrum(m_Chien);
	
	
	// if(!m_IsInCinematic)
	// {
	// 	UpdateSplineCamera();
	// 	UpdateCharactersInFrustrum(m_Berger);
	// 	UpdateCharactersInFrustrum(m_Chien);
	// }
}

void ASplineCameraScroller::InitializedSplineCameraScroller(TArray<AStateCharacter*> Characters)
{
	if(Characters.Num() < 2) return;
	if(!Characters[0] || !Characters[1]) return;
	
	m_Berger = Cast<AWoolStateCharacter>(Characters[0]);
	m_Chien = Cast<AWoolStateCharacter>(Characters[1]);
	
	
	if(!m_CameraActor)
	{
		//m_CameraActor = Cast<ACustomCameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomCameraActor::StaticClass()));
	}

	m_Berger->m_CameraActor = m_CameraActor;
	m_Chien->m_CameraActor = m_CameraActor;
}

void ASplineCameraScroller::UpdateSplineCamera()
{
	if(!m_Berger || !m_Chien || !m_CameraActor) return;

	// float CharactersDistance = FVector::Distance(m_Berger->m_CameraActor->GetActorLocation(), m_Chien->GetActorLocation());
	//
	// FVector MyLocation = m_CameraActor->GetActorLocation();
	// Direction = (InterpolatedLocation - MyLocation).GetSafeNormal();
	//
	// float Offset = FMath::Lerp(0, 100, CharactersDistance);
	// CameraTargetLocation = CameraTargetLocation + -Direction * Offset;




	
	// FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);
	// FVector CameraTargetLocation = m_SplineComponent->FindLocationClosestToWorldLocation(InterpolatedLocation, ESplineCoordinateSpace::World);
	//
	// FVector LerpedLocation = FMath::Lerp(m_CameraActor->GetActorLocation(), CameraTargetLocation, m_CameraMoveSpeed * GetWorld()->GetDeltaSeconds());
	//
	// m_CameraActor->SetActorLocation(LerpedLocation);








	FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);
	FVector CameraTargetLocation = m_SplineComponent->FindLocationClosestToWorldLocation(InterpolatedLocation, ESplineCoordinateSpace::World);

	float DistanceBetweenActors = FVector::Dist(m_Berger->GetActorLocation(), m_Chien->GetActorLocation());
	//float ZoomOutFactor = FMath::Clamp(DistanceBetweenActors * 1.0f, 0.f, 1000.f);
	float ZoomOutFactor = 0;

	FVector OffsetDirection = (CameraTargetLocation - InterpolatedLocation).GetSafeNormal(); 
	FVector AdjustedCameraTargetLocation = CameraTargetLocation + OffsetDirection * ZoomOutFactor;

	FVector LerpedLocation = FMath::Lerp(m_CameraActor->GetActorLocation(), AdjustedCameraTargetLocation, m_CameraMoveSpeed * GetWorld()->GetDeltaSeconds());

	m_CameraActor->SetActorLocation(LerpedLocation);
	
	LookAtTargetSmooth(InterpolatedLocation);
}

void ASplineCameraScroller::UpdateCharactersInFrustrum(ACharacter* Character)
{
	if(Character == nullptr)return;
	
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
        
	if (PlayerController)
	{
		FVector PlayerWorldPosition = Character->GetActorLocation();
		FVector2D ScreenPosition;
	
		bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(PlayerWorldPosition, ScreenPosition);
	
		if (bIsOnScreen)
		{
			// Get viewport size
			int32 ScreenWidth, ScreenHeight;
			PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);
	
			float Margin = 0.1f;
			float MinX = ScreenWidth * Margin;
			float MaxX = ScreenWidth * (1.0f - Margin);
			float MinY = ScreenHeight * Margin;
			float MaxY = ScreenHeight * (1.0f - Margin);
	
			// If Is going out of Camera Frustrum
			if (ScreenPosition.X < MinX || ScreenPosition.X > MaxX || ScreenPosition.Y < MinY || ScreenPosition.Y > MaxY)
			{

				FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);
				FVector CurrentDirection = InterpolatedLocation - Character->GetActorLocation();
				
				Character->AddMovementInput(CurrentDirection, 1.0f);
				
				// GEngine->AddOnScreenDebugMessage(
				// -1,
				// 3.0f,
				// FColor::Cyan,
				// "In Characters In Frustrum"
				// );
				//
				// //Direction = Direction.GetSafeNormal();
				// FVector Force = Direction * 25.0f;
				//
				// Character->GetCapsuleComponent()->SetSimulatePhysics(true);
				// Character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("PhysicsActor"));
				//
				// Character->GetCapsuleComponent()->SetLinearDamping(1.2f);
				// Character->GetCapsuleComponent()->SetAngularDamping(4.0f);
				//
				// // Apply force continuously (e.g., in Tick or with a timer)
				// Character->GetCapsuleComponent()->AddForce(Force);
			}
		}
	}
}

void ASplineCameraScroller::LookAtTargetSmooth(FVector TargetLocation)
{
	FVector MyLocation = m_CameraActor->GetActorLocation();
	Direction = (TargetLocation - MyLocation).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();


	
	//FRotator NewRotation = FMath::RInterpTo(m_CameraActor->GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), m_CameraRotationSpeed);
	FRotator NewRotation = FMath::Lerp(m_CameraActor->GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds() * m_CameraRotationSpeed);

	m_CameraActor->SetActorRotation(NewRotation);
}

void ASplineCameraScroller::EnterCinematic()
{
	m_IsInCinematic = true;
}

void ASplineCameraScroller::ExitCinematic()
{
	m_IsInCinematic = false;
	

	if(!m_Berger || !m_Chien || !m_CameraActor) return;
	
	FVector InterpolatedLocation = FMath::Lerp(m_Berger->GetActorLocation(), m_Chien->GetActorLocation(), 0.5f);
	FVector CameraTargetLocation = m_SplineComponent->FindLocationClosestToWorldLocation(InterpolatedLocation, ESplineCoordinateSpace::World);
	
	float DistanceBetweenActors = FVector::Dist(m_Berger->GetActorLocation(), m_Chien->GetActorLocation());
	//float ZoomOutFactor = FMath::Clamp(DistanceBetweenActors * 1.0f, 0.f, 1000.f);
	float ZoomOutFactor = 0;
	
	FVector OffsetDirection = (CameraTargetLocation - InterpolatedLocation).GetSafeNormal(); 
	FVector AdjustedCameraTargetLocation = CameraTargetLocation + OffsetDirection * ZoomOutFactor;
	
	m_CameraActor->SetActorLocation(AdjustedCameraTargetLocation);
	
	
	FVector MyLocation = m_CameraActor->GetActorLocation();
	Direction = (InterpolatedLocation - MyLocation).GetSafeNormal();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	
	m_CameraActor->SetActorRotation(LookAtRotation);
	
	

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if(PC)
	{
		//PC->SetViewTargetWithBlend(m_CameraActor, m_BlendingTime);

		PC->SetViewTarget(m_CameraActor);
	}
}

