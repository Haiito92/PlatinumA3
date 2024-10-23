// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/PDA/PDA_StateDatas.h"


// Sets default values for this component's properties
USmashCharacterStateJump::USmashCharacterStateJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	TObjectPtr<UAnimMontage> AnimMontage;
    AnimMontage = Character->GetStateDatas(GetStateID())->AnimMontage;
	
	if(AnimMontage)
	{
		Character->PlayAnimMontage(AnimMontage);
	}
	
	 GEngine->AddOnScreenDebugMessage(
	 	-1,
	 	3.0f,
	 	FColor::Cyan,
	 	TEXT("Enter State Jump")
	 );

	Character->Jump();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	 GEngine->AddOnScreenDebugMessage(
	 	-1,
	 	3.0f,
	 	FColor::Red,
	 	TEXT("Exit State Jump")
	 );

	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnInputMoveXFast);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Green,
	// 	TEXT("Tick State Jump")
	// );
	
}


void USmashCharacterStateJump::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}
