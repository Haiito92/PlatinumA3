// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/AISheepInPenBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/SheepSystem/SheepComponent.h"
#include "Runtime/SheepSystem/SheepPenSystemSettings.h"
#include "Runtime/SheepSystem/SheepStateFlag.h"

#pragma region BehaviourDefaults

void UAISheepInPenBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	SheepPenSystemSettings = GetDefault<USheepPenSystemSettings>();

	SheepComponents.Init(nullptr, Pawns.Num());

	for (int i = 0; i < Pawns.Num(); ++i)
	{
		const AAIGroupCharacter* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;

		USheepComponent* SheepComponent = Pawn->FindComponentByClass<USheepComponent>();
		if(SheepComponent == nullptr) continue;

		SheepComponents[i] = SheepComponent;
	}
}

bool UAISheepInPenBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	const USheepComponent* SheepComponent = SheepComponents[Pawn->GetIndex()];
	if(SheepComponent == nullptr) return false;

	return SheepComponent->GetIsCaptured();
}

void UAISheepInPenBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	if(SheepPenSystemSettings == nullptr) return;

	UCharacterMovementComponent* CharacterMovement = Pawn->GetCharacterMovement();
	if(CharacterMovement == nullptr) return;

	CharacterMovement->MaxWalkSpeed = SheepPenSystemSettings->SheepSpeedInPen;
	CharacterMovement->bCanWalkOffLedges = SheepPenSystemSettings->CanSheepWalkOffLedges;

	Pawn->StartMovingAICharacter();
	// Super::BehaviourEntry(Pawn);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Green,
	// TEXT("SheepInPen ENTRY"));
}

void UAISheepInPenBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	const USheepComponent* SheepComponent = SheepComponents[Pawn->GetIndex()];
	if(SheepComponent == nullptr) return;

	FVector NextLocation = SheepComponent->GetCapturedRoamingLocation();

	FVector Direction = NextLocation - Pawn->GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
			Pawn->GetActorLocation() + Direction);
	
	Pawn->StartRotateAICharacter(LookAtRotation);
	Pawn->AddMovementInput(Direction, 1.0f);
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Green,
	// TEXT("SheepInPen UPDATE"));
}

void UAISheepInPenBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	Pawn->StopRotateAICharacter();
	Pawn->StopMovingAICharacter();
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Green,
	// TEXT("SheepInPen EXIT"));
}

#pragma endregion 
