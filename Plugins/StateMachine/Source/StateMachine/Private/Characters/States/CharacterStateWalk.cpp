// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CharacterStateWalk.h"

#include "Characters/StateCharacter.h"
#include "Characters/StateCharacterSettings.h"
#include "Characters/CharacterStateMachine.h"
#include "Characters/PDA/PDA_StateDatas.h"


// Sets default values for this component's properties
UCharacterStateWalk::UCharacterStateWalk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


ECharacterStateID UCharacterStateWalk::GetStateID()
{
	return ECharacterStateID::Walk;
}

void UCharacterStateWalk::StateEnter(ECharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(Character->GetStateDatas(GetStateID())->AnimMontage);
	
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Cyan,
	// 	TEXT("Enter State Walk")
	// );
	
}

void UCharacterStateWalk::StateExit(ECharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Red,
	// 	TEXT("Exit State Walk")
	// );
	
}

void UCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Walk")
	);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	0.1f,
	// 	FColor::Green,
	// 	TEXT("Input Length: %f", *Character->GetInputMoves().GetSafeNormal().Length())
	// );

	UE_LOG(LogTemp, Warning, TEXT("InputMove Length: %f, Threshold: %f"), Character->GetInputMoves().Length(), CharacterSettings->InputMoveXThreshold);

	if(FMath::Abs(Character->GetInputMoves().Length()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ECharacterStateID::Idle);
	}else
	{
	
		// Move on Forward Backward
		Character->AddMovementInput(Character->GetActorRightVector(),  Character->GetInputMoves().X * Character->GetStateDatas(GetStateID())->GetFloatVariable("MoveSpeed") * DeltaTime);

		// Move on Left Right
		Character->AddMovementInput(Character->GetActorForwardVector(),  Character->GetInputMoves().Y * Character->GetStateDatas(GetStateID())->GetFloatVariable("MoveSpeed") * DeltaTime);

	}
}