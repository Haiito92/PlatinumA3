// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterState.h"
#include "CharacterStateRun.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STATEMACHINE_API UCharacterStateRun : public UCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStateRun();

	virtual ECharacterStateID GetStateID() override;

	virtual void StateEnter(ECharacterStateID PreviousStateID) override;

	virtual void StateExit(ECharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

};
