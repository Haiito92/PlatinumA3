// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SheepSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PLATINUMA3_API USheepSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void InitSubsystem();
	
#pragma region SheepCounting

public:
	
	UFUNCTION()
	void AddCapturedSheep(const unsigned int Amount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddCapturedSheepEvent, unsigned int, AmountAdded);
	FAddCapturedSheepEvent AddCapturedSheepEvent;
	
	UFUNCTION()
	unsigned int GetSheepCapturedCount() const;
	UFUNCTION()
	void SetSheepCapturedCount(const unsigned int NewValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSheepCapturedCountChangedEvent, unsigned int, SheepCapturedCount);
	FSheepCapturedCountChangedEvent SheepCapturedCountChangedEvent;
private:
	UPROPERTY()
	unsigned int SheepCapturedCount;
	
	
#pragma endregion 
};
