// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIIdleBehaviour.h"

#include "EditorMetadataOverrides.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Runtime/AIGroupSystem/AIDefaultBehavioursSettings.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"

#pragma region Behaviour Defaults

void UAIIdleBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	IdlingPawnDatas.Init({},Pawns.Num());
}

bool UAIIdleBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	return true;
}

void UAIIdleBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
	Super::BehaviourEntry(Pawn);
	int Index = Pawn->GetIndex();
	FIdlePawnData& Data = IdlingPawnDatas[Index];

	const UAIDefaultBehavioursSettings* AIDefaultBehavioursSettings = GetDefault<UAIDefaultBehavioursSettings>();
	if(AIDefaultBehavioursSettings == nullptr) return;
	
	Data.Timer = AIDefaultBehavioursSettings->DirectionChangeTime;
	GivePawnNewDirection(Data);

	

	UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = AIDefaultBehavioursSettings->WalkSpeed;
	}
}

void UAIIdleBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	int Index = Pawn->GetIndex();
	
	FIdlePawnData& Data = IdlingPawnDatas[Index];

	Data.Timer -= DeltaTime;
	if(Data.Timer <= 0.0f)
	{
		Data.Timer = 3.0f;
		GivePawnNewDirection(Data);
	}
	
	Pawn->AddMovementInput(Data.IdlingDirection);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("IDLE"));
}

void UAIIdleBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
{
	Super::BehaviourExit(Pawn);

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
	FVector randDir = FMath::VRand();
	randDir.Z=0;
	randDir.Normalize();
	Data.IdlingDirection = randDir;
}
