// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CharacterStateIdle.h"

#include "Characters/StateCharacter.h"
#include "Characters/StateCharacterSettings.h"
#include "Characters/CharacterStateMachine.h"
#include "Characters/PDA/PDA_StateDatas.h"


// Sets default values for this component's properties
UCharacterStateIdle::UCharacterStateIdle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ECharacterStateID UCharacterStateIdle::GetStateID()
{
	return ECharacterStateID::Idle;
}

void UCharacterStateIdle::StateEnter(ECharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(Character->GetStateDatas(GetStateID())->AnimMontage);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Cyan,
	// 	TEXT("Enter State Idle")
	// );
	
}

void UCharacterStateIdle::StateExit(ECharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Red,
	// 	TEXT("Exit State Idle")
	// );
}

void UCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);


	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Green,
	// 	TEXT("Tick State Idle")
	// );


	if(FMath::Abs(Character->GetInputMoves().GetSafeNormal().Length()) > CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ECharacterStateID::Walk);
	}
	
}