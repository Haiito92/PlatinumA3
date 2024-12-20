// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIIdleBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"
#include "Runtime/AIGroupSystem/AIDefaultBehavioursSettings.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

#pragma region Behaviour Defaults

void UAIIdleBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	IdlingPawnDatas.Init({},Pawns.Num());
}

bool UAIIdleBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	return Pawn->GetPawnStateID() == EAIPawnStateID::Activated;
}

void UAIIdleBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);
	
	const int Index = Pawn->GetIndex();
	FIdlePawnData& Data = IdlingPawnDatas[Index];

	const UAIDefaultBehavioursSettings* AIDefaultBehavioursSettings = GetDefault<UAIDefaultBehavioursSettings>();
	if(AIDefaultBehavioursSettings == nullptr) return;

	Data.IdleAnchorPosition = Pawn->GetActorLocation();
	Data.LastIdleEndPosition = Pawn->GetActorLocation();
	Data.Timer = AIDefaultBehavioursSettings->DirectionChangeTime;
	GivePawnNewDirection(Data);
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
					Pawn->GetActorLocation() + Data.IdlingDirection);
	Pawn->StartRotateAICharacter(LookAtRotation);
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = AIDefaultBehavioursSettings->WalkSpeed;
		MovementComponent->bCanWalkOffLedges = AIDefaultBehavioursSettings->CanIdleWalkOffLedges;
	}

	Pawn->StartMovingAICharacter();
}

void UAIIdleBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);
	
	const UAIDefaultBehavioursSettings* AIDefaultBehavioursSettings = GetDefault<UAIDefaultBehavioursSettings>();
	if(AIDefaultBehavioursSettings == nullptr) return;
	
	const int Index = Pawn->GetIndex();
	
	FIdlePawnData& Data = IdlingPawnDatas[Index];

	Data.Timer -= DeltaTime;
	
	if(Data.Timer <= 0.0f
		|| FVector::Distance(Data.IdlingDirection, Pawn->GetActorLocation()) <= 20.0f)
	{
		Data.LastIdleEndPosition = Pawn->GetActorLocation();
		Data.Timer = AIDefaultBehavioursSettings->DirectionChangeTime;
		GivePawnNewDirection(Data);
		
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
				Pawn->GetActorLocation() + Data.IdlingDirection);
		Pawn->StartRotateAICharacter(LookAtRotation);
	}

	
	
	Pawn->AddMovementInput(Data.IdlingDirection,1.0f);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("IDLE"));
}

void UAIIdleBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	Pawn->StopRotateAICharacter();
	Pawn->StopMovingAICharacter();
	//int Index = Pawn->GetIndex();
	//FIdlePawnData& Data = IdlingPawnDatas[Index];
	//Data.TimerHandle.Invalidate();
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("IDLE EXIT"));
}

#pragma endregion
void UAIIdleBehaviour::GivePawnNewDirection(FIdlePawnData& Data)
{
	FVector Location;

	const UAIDefaultBehavioursSettings* Settings = GetDefault<UAIDefaultBehavioursSettings>();
	if(Settings == nullptr) return;
	
	const float Radius = GetDefault<UAIDefaultBehavioursSettings>()->IdleRadius;
	
	Location.X = Data.IdleAnchorPosition.X + FMath::RandRange(-Radius, Radius); 
	Location.Y = Data.IdleAnchorPosition.Y + FMath::RandRange(-Radius, Radius); 
	Location.Z = Data.IdleAnchorPosition.Z;
	
	FVector randDir = Location - Data.LastIdleEndPosition;
	// UE_LOGFMT(LogTemp, Warning,"Ilde Location : X:{O}, Y:{1}, Z:{2}",
	// 	Location.X,Location.Y,Location.Z);


	randDir.Z=0;
	randDir.Normalize();
	Data.IdlingDirection = randDir;
}
