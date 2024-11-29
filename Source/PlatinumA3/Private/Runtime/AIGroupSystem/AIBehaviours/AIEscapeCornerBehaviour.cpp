// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIEscapeCornerBehaviour.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

void UAIEscapeCornerBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

}

bool UAIEscapeCornerBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> FoundActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Pawn->GetActorLocation(),
		150.0f,
		ObjectTypes,
		AActor::StaticClass(),
		ActorsToIgnore,
		FoundActors);
	
	return Super::CheckBehaviourValidity(Pawn);
}

void UAIEscapeCornerBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Orange,
		TEXT("Escape Corner Entry"));
}

void UAIEscapeCornerBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);
}

void UAIEscapeCornerBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Orange,
		TEXT("Escape Corner Exit"));
}
