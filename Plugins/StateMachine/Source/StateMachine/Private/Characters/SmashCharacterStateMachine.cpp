// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateMachine.h"
#include "InputMappingContext.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterState.h"

void USmashCharacterStateMachine::Tick(float DeltaTime)
{
	if(CurrentState != nullptr)
	CurrentState->StateTick(DeltaTime);
}

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();

	ChangeState(ESmashCharacterStateID::Idle);
}

ASmashCharacter* USmashCharacterStateMachine::GetSmashCharacter() const
{
	return Character;
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NextStateID)
{
	USmashCharacterState* NextState = GetState(NextStateID);

	if(NextState == nullptr) return;

	if(CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	ESmashCharacterStateID PreviousStateID = CurrentStateID;

	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if(CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID);
	}

	OnChangedState.Broadcast(NextStateID);
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID)
{
	for(USmashCharacterState* State : AllStates)
	{
		if(StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

void USmashCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(USmashCharacterState::StaticClass());
	for(UActorComponent* StateComponent : FoundComponents)
	{
		USmashCharacterState* State = Cast<USmashCharacterState>(StateComponent);
		if(State == nullptr) continue;
		if (State->GetStateID() == ESmashCharacterStateID::None) continue;

		AllStates.Add(State);
	}
}

void USmashCharacterStateMachine::InitStates()
{
	for (USmashCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
