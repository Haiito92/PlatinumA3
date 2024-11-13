// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/FleeSystem/FleeSystemSettings.h"

void UAIFleeBehaviour::InitBehaviour(const TArray<AAIGroupCharacter*>& Pawns)
{
	Super::InitBehaviour(Pawns);

	
	UFleeSubsystem * tFleeSubsystem = GetWorld()->GetSubsystem<UFleeSubsystem>();
	if(tFleeSubsystem != nullptr)
	{
		FleeSubsystem = tFleeSubsystem;
		// GEngine->AddOnScreenDebugMessage(
		// -1,
		// 3.0f,
		// FColor::Purple,
		// TEXT("Get FLEE Subsystem"));
	}
	
	const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return;
	const FName& Tag = FleeSystemSettings->TagToFleeFrom;
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	ActorsToFleeFrom = FoundActors;

	FleeLeaderComponents.Init(nullptr, Pawns.Num());

	for (int i = 0; i < Pawns.Num(); ++i)
	{
		UFleeLeaderComponent* FleeLeaderComponent = Pawns[i]->FindComponentByClass<UFleeLeaderComponent>();
		if(FleeLeaderComponent == nullptr) continue;

		FleeLeaderComponents[i] = FleeLeaderComponent;
	}
	
}

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupCharacter* Pawn) const
{
	bool valid = false;

	const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return false;
	
	for (AActor* Actor : ActorsToFleeFrom)
	{
		if(FVector::Distance(Pawn->GetActorLocation(), Actor->GetActorLocation()) < FleeSystemSettings->FleeDetectionRadius)
		{
			valid = true;
		}
	}
	
	return valid;
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupCharacter* Pawn)
{
	Super::BehaviourEntry(Pawn);
	
	UFleeLeaderComponent* LeaderComponent = FleeLeaderComponents[Pawn->GetIndex()];
	if(FleeSubsystem == nullptr || LeaderComponent == nullptr) return;
	FleeSubsystem->GetCurrentFleeLeaders().Add(Pawn->GetIndex(),LeaderComponent);
	
	const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return;
	
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = FleeSystemSettings->FleeSpeed;
	}
	
	
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("FLEE ENTRY"));
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupCharacter* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	FVector Direction = FVector::Zero();

	const FVector PawnLocation = Pawn->GetActorLocation();

	const UFleeSystemSettings* FleeSystemSettings = GetDefault<UFleeSystemSettings>();
	if(FleeSystemSettings == nullptr) return;
	
	for (AActor* ActorToFleeFrom : ActorsToFleeFrom)
	{
		FVector AtoP = PawnLocation - ActorToFleeFrom->GetActorLocation();
		if(AtoP.Length() < FleeSystemSettings->FleeDetectionRadius)
		{
			Direction += AtoP;
		}
	}

	Direction.Z = 0;
	Direction.Normalize();
	
	Pawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PawnLocation, PawnLocation+Direction));
	Pawn->AddMovementInput(Direction,1.0f);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	4.0f,
	// 	FColor::Orange,
	// 	TEXT("FLEE"));
}
void UAIFleeBehaviour::BehaviourExit(AAIGroupCharacter* Pawn)
{
	Super::BehaviourExit(Pawn);
	
	if(FleeSubsystem == nullptr) return;
	FleeSubsystem->GetCurrentFleeLeaders().Remove(Pawn->GetIndex());	
	GEngine->AddOnScreenDebugMessage(
	-1,
	4.0f,
	FColor::Orange,
	TEXT("FLEE EXIT"));
}
