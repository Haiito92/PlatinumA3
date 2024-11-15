// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class EGameStateID : uint8
{
 None,
 Not_Finishable,
 Finishable,
 Finished
};
