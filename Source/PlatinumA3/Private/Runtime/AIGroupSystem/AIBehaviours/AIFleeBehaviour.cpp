// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIBehaviourSettings.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"

void UAIFleeBehaviour::InitBehaviour()
{
	Super::InitBehaviour();


	const UAIBehaviourSettings* BehaviourSettings = GetDefault<UAIBehaviourSettings>();
	if(BehaviourSettings == nullptr) return;
	const FName& Tag = BehaviourSettings->TagToFleeFrom;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	ActorToFleeFrom = FoundActors;
}

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	bool valid = false;

	for (AActor* Actor : ActorToFleeFrom)
	{
		if(FVector::Distance(Pawn->GetActorLocation(), Actor->GetActorLocation()) < 300.0f)
		{
			valid = true;
		}
	}
	
	return valid;
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupPawn* Pawn) const
{
	Super::BehaviourEntry(Pawn);

	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FLEE ENTRY"));
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn) const
{
	Super::BehaviourUpdate(Pawn);

	Pawn->AddMovementInput(FVector::ForwardVector, 100);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	4.0f,
	// 	FColor::Orange,
	// 	TEXT("FLEE"));
}

void UAIFleeBehaviour::BehaviourExit(AAIGroupPawn* Pawn) const
{
	Super::BehaviourExit(Pawn);
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FLEE EXIT"));
}
