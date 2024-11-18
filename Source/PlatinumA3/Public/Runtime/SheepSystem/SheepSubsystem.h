// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SheepComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "SheepSubsystem.generated.h"

class USheepComponent;
/**
 * 
 */
UCLASS()
class PLATINUMA3_API USheepSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void InitSubsystem(const unsigned InSheepAmountRequired);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSheepSystemInitEvent);
	UPROPERTY()
	FSheepSystemInitEvent SheepSystemInitEvent;


	
#pragma region Sheeps

public:
	UFUNCTION()
	void AddSheep(USheepComponent* SheepComponent);
	
	UFUNCTION()
	const TArray<USheepComponent*>& GetSheeps() const;
protected:
	UPROPERTY()
	TArray<TObjectPtr<USheepComponent>> Sheeps;
#pragma endregion 
	
#pragma region Sheeps Counting

public:	
	UFUNCTION()
	void AddCapturedSheep(const unsigned int Amount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddCapturedSheepEvent, unsigned int, AmountAdded);
	UPROPERTY()
	FAddCapturedSheepEvent AddCapturedSheepEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachedRequiredSheepAmountEvent);
	UPROPERTY()
	FReachedRequiredSheepAmountEvent ReachedRequiredSheepAmountEvent;

	
	void DecrementSheepAliveCount(const unsigned int AmountToDecrement);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotEnoughSheepLeftEvent);
	UPROPERTY()
	FNotEnoughSheepLeftEvent NotEnoughSheepLeftEvent;

	UFUNCTION()
	unsigned int GetSheepAliveCount() const;
	UFUNCTION()
	void SetSheepAliveCount(const unsigned int NewAmount);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSheepAliveCountUpdatedEvent, unsigned int, SheepAliveCount);
	UPROPERTY()
	FSheepAliveCountUpdatedEvent SheepAliveCountUpdatedEvent;
	
	UFUNCTION()
	unsigned int GetSheepAmountRequired() const;
	UFUNCTION()
	void SetSheepAmountRequired(const unsigned int NewAmount);
	
	UFUNCTION()
	unsigned int GetSheepCapturedCount() const;
	UFUNCTION()
	void SetSheepCapturedCount(const unsigned int NewValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSheepCapturedCountChangedEvent, unsigned int, SheepCapturedCount);
	UPROPERTY()
	FSheepCapturedCountChangedEvent SheepCapturedCountChangedEvent;

	
protected:
	UPROPERTY()
	unsigned int SheepAliveCount;
	UPROPERTY()
	unsigned int SheepCapturedCount;
	UPROPERTY()
	unsigned int SheepAmountRequired;
	
	
#pragma endregion

#pragma region ReactionsToSheepEvents
	UFUNCTION()
	void OnSheepDeath(int Index);
#pragma endregion 
};
