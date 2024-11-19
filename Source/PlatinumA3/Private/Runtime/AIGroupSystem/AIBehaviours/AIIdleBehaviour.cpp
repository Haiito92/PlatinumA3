// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIIdleBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIDefaultBehavioursSettings.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

#pragma region Behaviour Defaults

void UAIIdleBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	IdlingPawnDatas.Init({},Pawns.Num());
}

bool UAIIdleBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	return true;
}

void UAIIdleBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);
	int Index = Pawn->GetIndex();
	FIdlePawnData& Data = IdlingPawnDatas[Index];

	const UAIDefaultBehavioursSettings* AIDefaultBehavioursSettings = GetDefault<UAIDefaultBehavioursSettings>();
	if(AIDefaultBehavioursSettings == nullptr) return;
	
	Data.Timer = AIDefaultBehavioursSettings->DirectionChangeTime;
	GivePawnNewDirection(Data);
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = AIDefaultBehavioursSettings->WalkSpeed;
		MovementComponent->bCanWalkOffLedges = AIDefaultBehavioursSettings->CanIdleWalkOffLedges;
	}
}

void UAIIdleBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
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

	Pawn->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
			Pawn->GetActorLocation() + Data.IdlingDirection)
			);
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
