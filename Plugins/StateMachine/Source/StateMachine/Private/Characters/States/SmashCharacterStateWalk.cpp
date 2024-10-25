// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateWalk.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "Characters/PDA/PDA_StateDatas.h"


// Sets default values for this component's properties
USmashCharacterStateWalk::USmashCharacterStateWalk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
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

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Red,
	// 	TEXT("Exit State Walk")
	// );
	
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
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
	
	if(FMath::Abs(Character->GetInputMoves().GetSafeNormal().Length()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}else
	{
		//Character->SetOrientX(Character->GetInputMoves().Normalize());
		
		// Move on Forward Backward
		Character->AddMovementInput(Character->GetActorRightVector(),  Character->GetInputMoves().X * Character->GetStateDatas(GetStateID())->GetFloatVariable("MoveSpeed") * DeltaTime);

		// Move on Left Right
		Character->AddMovementInput(Character->GetActorForwardVector(),  Character->GetInputMoves().Y * Character->GetStateDatas(GetStateID())->GetFloatVariable("MoveSpeed") * DeltaTime);

	}
}