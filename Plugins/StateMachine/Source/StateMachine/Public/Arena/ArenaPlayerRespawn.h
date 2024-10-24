// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaPlayerStartOrient.h"
#include "GameFramework/PlayerStart.h"
#include "ArenaPlayerRespawn.generated.h"

UCLASS()
class STATEMACHINE_API AArenaPlayerRespawn : public APlayerStart
{
	GENERATED_BODY()

public:
	UFUNCTION()
	float GetStartOrientX() const;

	UPROPERTY(EditAnywhere)
	EArenaPlayerStartOrient StartOrient = EArenaPlayerStartOrient::Right;
};
