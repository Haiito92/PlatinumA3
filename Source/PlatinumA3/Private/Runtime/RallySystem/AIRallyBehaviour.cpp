// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/AIRallyBehaviour.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/RallySystem/RallyReceiverComponent.h"
#include "Runtime/RallySystem/RallySystemSettings.h"

void UAIRallyBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
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

bool UAIRallyBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	return RallyReceiverComponents[Pawn->GetIndex()]->GetIsNotified();
}

void UAIRallyBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
	Super::BehaviourEntry(Pawn);

	const URallySystemSettings* RallySystemSettings = GetDefault<URallySystemSettings>();
	if(RallySystemSettings == nullptr) return;
	
	UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = RallySystemSettings->RallySpeed;
	}
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("RALLY ENTRY"));
}

void UAIRallyBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	const URallyReceiverComponent* RallyReceiverComponent = RallyReceiverComponents[Pawn->GetIndex()];
	FVector Direction = RallyReceiverComponent->GetDestination() - Pawn->GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();
	Pawn->AddMovementInput(Direction);
}

void UAIRallyBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
{
	Super::BehaviourExit(Pawn);

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("RALLY EXIT"));
}
