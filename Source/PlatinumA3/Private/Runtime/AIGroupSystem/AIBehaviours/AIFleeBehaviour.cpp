// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIFleeBehaviour.h"

#include "GameFramework/Character.h"

bool UAIFleeBehaviour::CheckBehaviourValidity() const
{
	return true;
}

void UAIFleeBehaviour::ApplyBehaviour(AAIGroupPawn* Character) const
{
	Super::ApplyBehaviour(Character);

	GEngine->AddOnScreenDebugMessage(
		-1,
		4.0f,
		FColor::Orange,
		TEXT("Character"));
}
