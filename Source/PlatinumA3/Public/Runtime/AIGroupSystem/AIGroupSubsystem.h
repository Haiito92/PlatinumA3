// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIGroupSubsystem.generated.h"

class AAIGroupPawn;
class UAIBehaviour;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API UAIGroupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TArray<AAIGroupPawn*> Characters;
	TArray<UAIBehaviour*> Behaviours;
	
public:
	void InitSubsystem();
	
private:
	void GroupUpdate() const;
};
