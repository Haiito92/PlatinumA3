// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(Blueprintable, BlueprintType)
enum class ESheepStateID : uint8
{
 None = 0,
 IdleWalk,
 Rally,
 Flee
};
