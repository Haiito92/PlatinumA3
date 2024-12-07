#include "Runtime/AIGroupSystem/AIGroupSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "Runtime/AIGroupSystem/AIGroupCharacter.h"
#include "Runtime/AIGroupSystem/AIGroupSubsystemSettings.h"
#include "Runtime/AIGroupSystem/AIPawnStateID.h"


#pragma region SystemInitialization

void UAIGroupSubsystem::InitSubsystem()
{
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Yellow,
	// 	TEXT("INIT AIGROUP SUBSYSTEM"));
	Settings = GetDefault<UAIGroupSubsystemSettings>();
	if(Settings == nullptr) return;

	UE_LOGFMT(LogTemp, Warning, "Init");
	InitPawnPool(Settings->PoolStartSize);
	
	FindAllPawns();
	InitAllPawns();
	
	InitBehaviours();
	InitPawnDatas();

	StartFirstPawnsBehavior();

	GetWorld()->GetTimerManager().SetTimer(
		GroupUpdateTimerHandle,
		this,
		&UAIGroupSubsystem::GroupUpdate,
		SYSTEM_UPDATE_RATE,
		true);
}

void UAIGroupSubsystem::InitBehaviours()
{

	if(Settings == nullptr) return;

	
	for (TSubclassOf<UAIBehaviour> BehaviorClass : Settings->BehaviorsClasses)
	{
		UAIBehaviour* Behaviour = NewObject<UAIBehaviour>(this, BehaviorClass);

		if(Behaviour == nullptr) continue;
		
		Behaviour->InitBehaviour(Pawns);
		Behaviours.Add(Behaviour);
	}
}

void UAIGroupSubsystem::FindAllPawns()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIGroupCharacter::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		AAIGroupCharacter* Pawn = Cast<AAIGroupCharacter>(FoundActor);
		if(Pawn == nullptr) return;
		Pawns.Add(Pawn);
	}
}

void UAIGroupSubsystem::InitAllPawns()
{
	for (int i = 0; i < Pawns.Num(); ++i)
	{
		AAIGroupCharacter* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;

		Pawn->InitGroupPawn(i);
	}
}

void UAIGroupSubsystem::InitPawnDatas()
{
	for (AAIGroupCharacter* Pawn : Pawns)
	{
		FAIGroupPawnData PawnData = {Behaviours.Last()};
		PawnDatas.Add(PawnData);
	}
}

void UAIGroupSubsystem::StartFirstPawnsBehavior()
{
	for (int i = 0; i < Pawns.Num(); ++i)
	{
		AAIGroupCharacter* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;

		const FAIGroupPawnData& Data = PawnDatas[i];
		UAIBehaviour* StartBehaviour = Data.CurrentBehaviour;
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
		AAIGroupCharacter* Pawn = Pawns[i];
		if(Pawn == nullptr) continue;

		//Check That pawn is activated
		if(Pawn->GetPawnStateID() != EAIPawnStateID::Activated) continue;
		
		FAIGroupPawnData& Data = PawnDatas[i];
		
		//Get first valid behaviour
		UAIBehaviour* NewBehaviour = FindFirstValidBehaviour(Pawn);
		if(NewBehaviour == nullptr)
		{
			Data.CurrentBehaviour = nullptr;
			continue;
		};

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
		Data.CurrentBehaviour->BehaviourUpdate(Pawn, SYSTEM_UPDATE_RATE);
	}
}

UAIBehaviour* UAIGroupSubsystem::FindFirstValidBehaviour(AAIGroupCharacter* Pawn) const
{
	for (UAIBehaviour* Behaviour : Behaviours)
	{
		if(Behaviour->CheckBehaviourValidity(Pawn)) return Behaviour;
	}

	return nullptr;
}
#pragma endregion

#pragma region SystemPool
void UAIGroupSubsystem::InitPawnPool(const int InStartPawnAmount)
{
	if(Settings == nullptr) return;
	UE_LOGFMT(LogTemp, Warning, "Init Pawn Pool");
	PoolLocation = {-100000,-100000, -100000};
	
	for (int i = 0; i < InStartPawnAmount; ++i)
	{
		AAIGroupCharacter* Pawn =
			GetWorld()->SpawnActor<AAIGroupCharacter>(Settings->PawnClass,PoolLocation, FRotator::ZeroRotator);
		if(Pawn == nullptr) continue;
		
		Pawn->UnActivatePawn();
		Pawns.Add(Pawn);
	}
}
#pragma endregion 