// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterStateMachine.generated.h"

/**
 * 
 */
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState, ECharacterStateID, StateID);


enum class ECharacterStateID : uint8;
class UCharacterState;
class AStateCharacter;

UCLASS()
class STATEMACHINE_API UCharacterStateMachine : public UObject
{
	GENERATED_BODY()



public:
	void Tick(float DeltaTime);

	
	void Init(AStateCharacter* InCharacter);

	AStateCharacter* GetSmashCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ECharacterStateID NextStateID);

	UCharacterState* GetState(ECharacterStateID StateID);
	
protected:
	UPROPERTY()
	TObjectPtr<AStateCharacter> Character;

	TArray<UCharacterState*> AllStates;


	UPROPERTY(BlueprintReadOnly)
	ECharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<UCharacterStateMachine> StateMachine;
	
	UPROPERTY()
	TObjectPtr<UCharacterState> CurrentState;

public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeState OnChangedState;
	
	void FindStates();

	void InitStates();
};
