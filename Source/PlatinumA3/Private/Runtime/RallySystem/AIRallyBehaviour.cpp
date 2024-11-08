// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/RallySystem/AIRallyBehaviour.h"

#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/RallySystem/RallyReceiverComponent.h"

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
