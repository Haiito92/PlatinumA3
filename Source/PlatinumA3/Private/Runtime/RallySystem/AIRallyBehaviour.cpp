// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/AIRallyBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/RallySystem/RallyReceiverComponent.h"
#include "Runtime/RallySystem/RallySystemSettings.h"

void UAIRallyBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);
	RallyReceiverComponents.Init(nullptr, Pawns.Num());

	for (int i = 0; i < Pawns.Num(); ++i)
	{
		URallyReceiverComponent* RallyReceiverComponent = Pawns[i]->FindComponentByClass<URallyReceiverComponent>();
		if(RallyReceiverComponent == nullptr) continue;

		RallyReceiverComponents[i] = RallyReceiverComponent;
	}
}

bool UAIRallyBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	return RallyReceiverComponents[Pawn->GetIndex()]->GetIsNotified();
}

void UAIRallyBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);

	const URallySystemSettings* RallySystemSettings = GetDefault<URallySystemSettings>();
	if(RallySystemSettings == nullptr) return;
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->bCanWalkOffLedges = RallySystemSettings->CanRallyWalkOffLedges;
		MovementComponent->MaxWalkSpeed = RallySystemSettings->RallySpeed;
	}
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("RALLY ENTRY"));
}

void UAIRallyBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	const URallyReceiverComponent* RallyReceiverComponent = RallyReceiverComponents[Pawn->GetIndex()];
	FVector Direction = RallyReceiverComponent->GetDestination() - Pawn->GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();

	//Set Sheep Rotation
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
			Pawn->GetActorLocation() + Direction);
	Pawn->StartRotateAICharacter(LookAtRotation);
	Pawn->AddMovementInput(Direction, 1.0f);
}

void UAIRallyBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	Pawn->StopRotateAICharacter();

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("RALLY EXIT"));
}
