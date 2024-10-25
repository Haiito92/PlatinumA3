// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterState.h"
#include "Characters/StateCharacterSettings.h"
#include "Characters/CharacterStateID.h"
#include "Characters/CharacterStateMachine.h"


// Sets default values for this component's properties
UCharacterState::UCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ECharacterStateID UCharacterState::GetStateID()
{
	return ECharacterStateID::None;
}

void UCharacterState::StateInit(UCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetSmashCharacter();
	CharacterSettings = GetDefault<UStateCharacterSettings>();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID())

	);
}

void UCharacterState::StateEnter(ECharacterStateID PreviousStateID)
{
}

void UCharacterState::StateExit(ECharacterStateID NextStateID)
{
}

void UCharacterState::StateTick(float DeltaTime)
{
	
}
