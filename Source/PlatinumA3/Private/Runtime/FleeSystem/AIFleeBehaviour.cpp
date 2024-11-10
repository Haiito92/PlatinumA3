// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/AIFleeBehaviour.h"

#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/AIGroupSystem/AIBehaviourSettings.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/FleeSystem/FleeLeaderComponent.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"

void UAIFleeBehaviour::InitBehaviour(const TArray<AAIGroupPawn*>& Pawns)
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
	
	const UAIBehaviourSettings* BehaviourSettings = GetDefault<UAIBehaviourSettings>();
	if(BehaviourSettings == nullptr) return;
	const FName& Tag = BehaviourSettings->TagToFleeFrom;
	
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

bool UAIFleeBehaviour::CheckBehaviourValidity(AAIGroupPawn* Pawn) const
{
	bool valid = false;

	for (AActor* Actor : ActorsToFleeFrom)
	{
		if(FVector::Distance(Pawn->GetActorLocation(), Actor->GetActorLocation()) < 400.0f)
		{
			valid = true;
		}
	}
	
	return valid;
}

void UAIFleeBehaviour::BehaviourEntry(AAIGroupPawn* Pawn)
{
	Super::BehaviourEntry(Pawn);

	UFloatingPawnMovement* MovementComponent = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent());
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxSpeed = 250.0f;
	}
	
	UFleeLeaderComponent* LeaderComponent = FleeLeaderComponents[Pawn->GetIndex()];
	if(FleeSubsystem == nullptr || LeaderComponent == nullptr) return;
	FleeSubsystem->GetCurrentFleeLeaders().Add(Pawn->GetIndex(),LeaderComponent);
	// GEngine->AddOnScreenDebugMessage(
	// -1,
	// 4.0f,
	// FColor::Orange,
	// TEXT("FLEE ENTRY"));
}

void UAIFleeBehaviour::BehaviourUpdate(AAIGroupPawn* Pawn, float DeltaTime)
{
	Super::BehaviourUpdate(Pawn, DeltaTime);

	FVector Direction = FVector::Zero();

	const FVector PawnLocation = Pawn->GetActorLocation();
	
	for (AActor* ActorToFleeFrom : ActorsToFleeFrom)
	{
		FVector AtoP = PawnLocation - ActorToFleeFrom->GetActorLocation();
		if(AtoP.Length() < 400.0f)
		{
			Direction += AtoP;
		}
	}

	Direction.Z = 0;
	Direction.Normalize();
	
	Pawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PawnLocation, PawnLocation+Direction));
	Pawn->AddMovementInput(Direction);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	4.0f,
	// 	FColor::Orange,
	// 	TEXT("FLEE"));
}
void UAIFleeBehaviour::BehaviourExit(AAIGroupPawn* Pawn)
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
