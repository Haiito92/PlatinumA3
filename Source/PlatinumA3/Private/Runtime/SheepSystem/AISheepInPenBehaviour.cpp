// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/AISheepInPenBehaviour.h"

#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/SheepSystem/SheepComponent.h"

#pragma region BehaviourDefaults

void UAISheepInPenBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

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

bool UAISheepInPenBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	const USheepComponent* SheepComponent = SheepComponents[Pawn->GetIndex()];
	if(SheepComponent == nullptr) return false;
	
	return SheepComponent->GetIsCaptured();
}

void UAISheepInPenBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Green,
	TEXT("SheepInPen ENTRY"));
}

void UAISheepInPenBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Green,
	// TEXT("SheepInPen UPDATE"));
}

void UAISheepInPenBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Green,
	TEXT("SheepInPen EXIT"));
}

#pragma endregion 
