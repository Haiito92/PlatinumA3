// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/UI/SheepCountUI.h"

#include "Runtime/SheepSystem/SheepSubsystem.h"

#pragma region Defaults	
void USheepCountUI::NativeConstruct()
{
	Super::NativeConstruct();

	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem == nullptr) return;

	SheepSubsystem->SheepCapturedCountChangedEvent.AddDynamic(
		this, &USheepCountUI::OnSheepCapturedCountChangedEvent);
}

void USheepCountUI::NativeDestruct()
{
	Super::NativeDestruct();

	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem == nullptr) return;

	SheepSubsystem->SheepCapturedCountChangedEvent.RemoveDynamic(
		this, &USheepCountUI::OnSheepCapturedCountChangedEvent);
}

void USheepCountUI::OnSheepCapturedCountChangedEvent(unsigned InSheepCapturedCount)
{
	UpdateSheepCapturedCountUI(InSheepCapturedCount);
}

void USheepCountUI::UpdateSheepCapturedCountUI(unsigned InSheepCapturedCount)
{
	SheepCapturedCount = InSheepCapturedCount;
}
#pragma endregion 
