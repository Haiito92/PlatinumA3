// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterStateMachine.h"
#include "InputMappingContext.h"

#include "Characters/StateCharacter.h"
#include "Characters/CharacterState.h"

void UCharacterStateMachine::Tick(float DeltaTime)
{
	if(CurrentState != nullptr)
	CurrentState->StateTick(DeltaTime);
}

void UCharacterStateMachine::Init(AStateCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();

	ChangeState(ECharacterStateID::Idle);
}

AStateCharacter* UCharacterStateMachine::GetSmashCharacter() const
{
	return Character;
}

void UCharacterStateMachine::ChangeState(ECharacterStateID NextStateID)
{
	UCharacterState* NextState = GetState(NextStateID);

	if(NextState == nullptr) return;

	if(CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	ECharacterStateID PreviousStateID = CurrentStateID;

	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if(CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID);
	}

	OnChangedState.Broadcast(NextStateID);
}

UCharacterState* UCharacterStateMachine::GetState(ECharacterStateID StateID)
{
	for(UCharacterState* State : AllStates)
	{
		if(StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

void UCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(UCharacterState::StaticClass());
	for(UActorComponent* StateComponent : FoundComponents)
	{
		UCharacterState* State = Cast<UCharacterState>(StateComponent);
		if(State == nullptr) continue;
		if (State->GetStateID() == ECharacterStateID::None) continue;

		AllStates.Add(State);
	}
}

void UCharacterStateMachine::InitStates()
{
	for (UCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
