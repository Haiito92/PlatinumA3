// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateID.h"
#include "Components/ActorComponent.h"
#include "CharacterState.generated.h"


class UStateCharacterSettings;
class AStateCharacter;
class UCharacterStateMachine;

UCLASS(Abstract)
class STATEMACHINE_API UCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterState();


	virtual ECharacterStateID GetStateID();

	virtual void StateInit(UCharacterStateMachine* InStateMachine);

	virtual void StateEnter(ECharacterStateID PreviousStateID);

	virtual void StateExit(ECharacterStateID NextStateID);

	virtual void StateTick(float DeltaTime);
protected:
	UPROPERTY()
	TObjectPtr<AStateCharacter> Character;

	UPROPERTY()
	TObjectPtr<UCharacterStateMachine> StateMachine;

	UPROPERTY()
	const UStateCharacterSettings* CharacterSettings;

};
