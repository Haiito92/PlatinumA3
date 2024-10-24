// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateWalk.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STATEMACHINE_API USmashCharacterStateWalk : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateWalk();

	virtual ESmashCharacterStateID GetStateID() override;

	UFUNCTION()
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

};
