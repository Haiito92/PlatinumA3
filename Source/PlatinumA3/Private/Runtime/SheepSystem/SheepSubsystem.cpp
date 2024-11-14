// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/SheepSubsystem.h"

void USheepSubsystem::InitSubsystem()
{
	SetSheepCapturedCount(0);
	SetSheepAmountRequired(4);
	SheepSystemInitEvent.Broadcast();
}

#pragma region SheepCounting
void USheepSubsystem::AddCapturedSheep(const unsigned int Amount)
{
	SetSheepCapturedCount(SheepCapturedCount + Amount);
	AddCapturedSheepEvent.Broadcast(Amount);

	if(SheepCapturedCount >= SheepAmountRequired)
	{
		ReachedRequiredSheepAmountEvent.Broadcast();
	}
}

unsigned USheepSubsystem::GetSheepAmountRequired() const
{
	return SheepAmountRequired;
}

void USheepSubsystem::SetSheepAmountRequired(const unsigned NewAmount)
{
	SheepAmountRequired = NewAmount;
}

unsigned USheepSubsystem::GetSheepCapturedCount() const
{
	return SheepCapturedCount;
}

void USheepSubsystem::SetSheepCapturedCount(const unsigned int NewValue)
{
	SheepCapturedCount = NewValue;
	SheepCapturedCountChangedEvent.Broadcast(SheepCapturedCount);
}
#pragma endregion 
