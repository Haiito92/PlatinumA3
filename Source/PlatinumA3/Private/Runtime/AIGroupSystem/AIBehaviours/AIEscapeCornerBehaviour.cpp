// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIBehaviours/AIEscapeCornerBehaviour.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/AIGroupSystem/AIDefaultBehavioursSettings.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

void UAIEscapeCornerBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	EscapeDatas.Init({}, Pawns.Num());

	Settings = GetDefault<UAIDefaultBehavioursSettings>();
}

bool UAIEscapeCornerBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn)
{
	if (Settings == nullptr) return false;
	
	UObject* World = GetWorld();
	
	const FVector Start = Pawn->GetActorLocation();
	TArray<FVector> Directions = {FVector::LeftVector,FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector};
	TArray<FVector> Ends;
	for (FVector Direction : Directions)
	{
		Ends.Add(Start + Direction * Settings->EscapeDetectionDistance);
	}

	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Pawn);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::Type::None;

	TArray<FHitResult> HitResults;

	for (FVector End : Ends)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(World, Start, End, TraceTypeQuery,
		false, ActorsToIgnore, DrawDebugType,HitResult, true);
		HitResults.Add(HitResult);
	}

	bool bValid = false;
	FVector Direction = FVector::ZeroVector;
	for (FHitResult HitResult : HitResults)
	{
		if(!HitResult.bBlockingHit)continue;
		
		bValid = true;
		Direction += HitResult.Normal;
		Direction.Z = 0;
		Direction.Normalize();
	}

	if(bValid == true)
	{
		EscapeDatas[Pawn->GetIndex()].EscapeDirection = Direction;
	}
	
	return bValid;
}

void UAIEscapeCornerBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Orange,
		TEXT("Escape Corner Entry"));

	if(Settings == nullptr) return;

	const FEscapeCornerPawnData& Data = EscapeDatas[Pawn->GetIndex()];
	
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(),
					Pawn->GetActorLocation() + Data.EscapeDirection);
	Pawn->StartRotateAICharacter(LookAtRotation);
	
	UCharacterMovementComponent* MovementComponent = Pawn->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = Settings->EscapeWalkSpeed;
		MovementComponent->bCanWalkOffLedges = Settings->CanEscapeWalkOffLedges;
	}
}

void UAIEscapeCornerBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);
	
	const FEscapeCornerPawnData& Data = EscapeDatas[Pawn->GetIndex()];

	Pawn->AddMovementInput(Data.EscapeDirection,1.0f);
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
