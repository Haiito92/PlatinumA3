// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/UI/SheepCountUI.h"

#include "Runtime/SheepSystem/SheepSubsystem.h"

#pragma region Defaults	
void USheepCountUI::NativeConstruct()
{
	Super::NativeConstruct();

	USheepSubsystem* tSheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(tSheepSubsystem == nullptr) return;
	SheepSubsystem = tSheepSubsystem;

	//SheepSubsystem->SheepSystemInitEvent.AddDynamic(this, &USheepCountUI::InitSheepCountUI);
	
	SheepSubsystem->SheepCapturedCountChangedEvent.AddDynamic(
		this, &USheepCountUI::OnSheepCapturedCountChangedEvent);

}

void USheepCountUI::NativeDestruct()
{
	Super::NativeDestruct();

	if(SheepSubsystem == nullptr) return;

	//SheepSubsystem->SheepSystemInitEvent.RemoveDynamic(this, &USheepCountUI::InitSheepCountUI);
	
	SheepSubsystem->SheepCapturedCountChangedEvent.RemoveDynamic(
		this, &USheepCountUI::OnSheepCapturedCountChangedEvent);
}

void USheepCountUI::InitSheepCountUI()
{
	UpdateSheepAmountRequiredUI(SheepSubsystem->GetSheepAmountRequired());
	UpdateSheepCapturedCountUI(SheepSubsystem->GetSheepCapturedCount());
}

void USheepCountUI::OnSheepCapturedCountChangedEvent(unsigned InSheepCapturedCount)
{
	UpdateSheepCapturedCountUI(InSheepCapturedCount);
}

void USheepCountUI::UpdateSheepAmountRequiredUI(const unsigned InAmount)
{
	SheepAmountRequired = InAmount;
}

void USheepCountUI::UpdateSheepCapturedCountUI(const unsigned InSheepCapturedCount)
{
	SheepCapturedCount = InSheepCapturedCount;
}
#pragma endregion 
