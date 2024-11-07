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
	InitPawnDatas();
	InitAllPawns();

	

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
		
		Behaviour->InitBehaviour();
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


void UAIGroupSubsystem::InitPawnDatas()
{
	for (AAIGroupPawn* Pawn : Pawns)
	{
		FAIGroupPawnData PawnData = {Behaviours.Last()};
		PawnDatas.Add(PawnData);
	}
}

void UAIGroupSubsystem::InitAllPawns()
{
	for (int i = 0; i < Pawns.Num(); ++i)
	{
		AAIGroupPawn* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;
		
		const FAIGroupPawnData& Data = PawnDatas[i];
		const UAIBehaviour* StartBehaviour = Data.CurrentBehaviour;
		if(StartBehaviour == nullptr) continue;

		StartBehaviour->BehaviourEntry(Pawn);
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
		UAIBehaviour* NewBehaviour = FindFirstValidBehaviour(Pawn);
		if(NewBehaviour == nullptr) continue;

		FAIGroupPawnData& Data = PawnDatas[i];
		if(Data.CurrentBehaviour == nullptr) continue;
			
		//Compare New Behavior with current one
		if(NewBehaviour != Data.CurrentBehaviour)
		{
			//Exit Previous Behaviour
			Data.CurrentBehaviour->BehaviourExit(Pawn);

			//Update CurrentState
			Data.CurrentBehaviour = NewBehaviour;

			//Entry NewBehaviour
			Data.CurrentBehaviour->BehaviourEntry(Pawn);
		}
		
		//Update current Behavior
		Data.CurrentBehaviour->BehaviourUpdate(Pawn);
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