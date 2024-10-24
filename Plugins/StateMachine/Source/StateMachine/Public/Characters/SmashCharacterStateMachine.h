// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashCharacterStateMachine.generated.h"

/**
 * 
 */
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeState, ESmashCharacterStateID, StateID);


enum class ESmashCharacterStateID : uint8;
class USmashCharacterState;
class ASmashCharacter;

UCLASS()
class STATEMACHINE_API USmashCharacterStateMachine : public UObject
{
	GENERATED_BODY()



public:
	void Tick(float DeltaTime);

	
	void Init(ASmashCharacter* InCharacter);

	ASmashCharacter* GetSmashCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ESmashCharacterStateID NextStateID);

	USmashCharacterState* GetState(ESmashCharacterStateID StateID);
	
protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	TArray<USmashCharacterState*> AllStates;


	UPROPERTY(BlueprintReadOnly)
	ESmashCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
	UPROPERTY()
	TObjectPtr<USmashCharacterState> CurrentState;

public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeState OnChangedState;
	
	void FindStates();

	void InitStates();
};
