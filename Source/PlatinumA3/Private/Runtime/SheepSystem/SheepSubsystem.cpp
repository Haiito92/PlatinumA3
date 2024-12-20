// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/SheepSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/WoolsomeGameInstance.h"
#include "Runtime/SheepSystem/SheepComponent.h"
#include "Runtime/SheepSystem/SheepPenSystemSettings.h"

void USheepSubsystem::InitSubsystem(const unsigned InSheepAmountRequired)
{
	const UWoolsomeGameInstance* GameInstance =
		Cast<UWoolsomeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	int tSheepAmountRequired = 3;
	if(GameInstance != nullptr) tSheepAmountRequired = GameInstance->GetCurrentLevelInfo().LevelSheepAmountToFinish;
	
	SetSheepCapturedCount(0);
	SetSheepAmountRequired(tSheepAmountRequired);

	const USheepPenSystemSettings* Settings = GetDefault<USheepPenSystemSettings>();
	if(Settings == nullptr) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),Settings->SheepTag,FoundActors);

	int BornSheeps = 0;
	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		USheepComponent* SheepComponent = FoundActors[i]->FindComponentByClass<USheepComponent>();
		if(SheepComponent == nullptr) continue;

		SheepComponent->InitSheep(i);
		AddSheep(SheepComponent);

		if(SheepComponent->GetIsAlive()) ++BornSheeps;
	}

	SetSheepAliveCount(BornSheeps);
	//UE_LOG(LogTemp, Warning, TEXT("Sheeps Alive : %d"), SheepAliveCount);

	SheepSystemInitEvent.Broadcast();
}

#pragma region Sheeps
void USheepSubsystem::AddSheep(USheepComponent* SheepComponent)
{
	SheepComponent->SheepBirthEvent.AddDynamic(this, &USheepSubsystem::OnSheepBirth);
	SheepComponent->SheepDeathEvent.AddDynamic(this, &USheepSubsystem::OnSheepDeath);
	
	Sheeps.Add(SheepComponent);
}



const TArray<USheepComponent*>& USheepSubsystem::GetSheeps() const
{
	return Sheeps;
}

#pragma endregion

#pragma region Sheeps Counting

void USheepSubsystem::AddCapturedSheep(const unsigned int Amount)
{
	SetSheepCapturedCount(SheepCapturedCount + Amount);
	AddCapturedSheepEvent.Broadcast(Amount);

	if(SheepCapturedCount >= SheepAmountRequired)
	{
		ReachedRequiredSheepAmountEvent.Broadcast();
	}
}

void USheepSubsystem::IncrementSheepAliveCount(const unsigned AmountToIncrement)
{
	SetSheepAliveCount(SheepAliveCount + AmountToIncrement);
}

void USheepSubsystem::DecrementSheepAliveCount(const unsigned AmountToDecrement)
{
	SetSheepAliveCount(SheepAliveCount - AmountToDecrement);

	if(SheepAliveCount < SheepAmountRequired)
	{
		NotEnoughSheepLeftEvent.Broadcast();
		// UE_LOG(LogTemp, Warning, TEXT("Not Enough Sheeps Alive Left"));
	}
}

unsigned USheepSubsystem::GetSheepAliveCount() const
{
	return SheepAliveCount;
}

void USheepSubsystem::SetSheepAliveCount(const unsigned NewAmount)
{
	if(SheepAliveCount == NewAmount) return;

	SheepAliveCount = NewAmount;

	SheepAliveCountUpdatedEvent.Broadcast(SheepAliveCount);
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

#pragma region ReactionsToSheepEvents

void USheepSubsystem::OnSheepBirth(int Index)
{
	
}

void USheepSubsystem::OnSheepDeath(int Index)
{
	DecrementSheepAliveCount(1);
}
#pragma endregion