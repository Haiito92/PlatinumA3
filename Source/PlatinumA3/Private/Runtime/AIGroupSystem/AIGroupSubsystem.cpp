#include "Runtime/AIGroupSystem/AIGroupSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "Runtime/AIGroupSystem/AIGroupPawn.h"
#include "Runtime/AIGroupSystem/AIGroupSubsystemSettings.h"


#pragma region SystemInitialization


void UAIGroupSubsystem::InitSubsystem()
{
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Yellow,
	// 	TEXT("INIT AIGROUP SUBSYSTEM"));
	
	InitBehaviours();

	FindAllPawns();
	InitAllPawns();
	InitPawnDatas();

	GetWorld()->GetTimerManager().SetTimer(
		GroupUpdateTimerHandle,
		this,
		&UAIGroupSubsystem::GroupUpdate,
		0.02f,
		true);
}

void UAIGroupSubsystem::InitBehaviours()
{
	const UAIGroupSubsystemSettings* Settings = GetDefault<UAIGroupSubsystemSettings>();

	if(Settings == nullptr) return;

	
	for (TSubclassOf<UAIBehaviour> BehaviorClass : Settings->BehaviorsClasses)
	{
		UAIBehaviour* Behaviour = NewObject<UAIBehaviour>(this, BehaviorClass);

		if(Behaviour == nullptr) continue;
		Behaviours.Add(Behaviour);
	}
}

void UAIGroupSubsystem::FindAllPawns()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIGroupPawn::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		AAIGroupPawn* Pawn = Cast<AAIGroupPawn>(FoundActor);
		if(Pawn == nullptr) return;
		Pawns.Add(Pawn);
	}
}

void UAIGroupSubsystem::InitAllPawns()
{
	for (AAIGroupPawn* Pawn : Pawns)
	{
		//Call Init function of pawn
	}
}

void UAIGroupSubsystem::InitPawnDatas()
{
	for (AAIGroupPawn* Pawn : Pawns)
	{
		FAIGroupPawnData PawnData = {Behaviours.Last()};
		PawnDatas.Add(PawnData);
	}
}
#pragma endregion 

#pragma region System Update
void UAIGroupSubsystem::GroupUpdate()
{
	
	if(Pawns.IsEmpty()) return;
	if(Behaviours.IsEmpty()) return;

	for (int i = 0; i < Pawns.Num(); ++i)
	{
		AAIGroupPawn* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;

		//Get first valid behaviour
		const UAIBehaviour* NewBehaviour = FindFirstValidBehaviour(Pawn);
		if(NewBehaviour == nullptr) continue;

		const FAIGroupPawnData& Data = PawnDatas[i];
		const UAIBehaviour* CurrentBehaviour = Data.CurrentBehaviour;
		if(CurrentBehaviour == nullptr) continue;
			
		//Compare New Behavior with current one
		if(NewBehaviour != CurrentBehaviour)
		{
			//Exit Previous Behaviour
			CurrentBehaviour->BehaviourExit(Pawn);

			//Update CurrentState
			CurrentBehaviour = NewBehaviour;

			//Entry NewBehaviour
			CurrentBehaviour->BehaviourEntry(Pawn);
		}
		
		//Update current Behavior
		CurrentBehaviour->BehaviourUpdate(Pawn);
	}
}

UAIBehaviour* UAIGroupSubsystem::FindFirstValidBehaviour(AAIGroupPawn* Pawn) const
{
	for (UAIBehaviour* Behaviour : Behaviours)
	{
		if(Behaviour->CheckBehaviourValidity(Pawn)) return Behaviour;
	}

	return nullptr;
}
#pragma endregion 