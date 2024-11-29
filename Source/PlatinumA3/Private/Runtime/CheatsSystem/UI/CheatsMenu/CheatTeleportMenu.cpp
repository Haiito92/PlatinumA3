// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/UI/CheatsMenu/CheatTeleportMenu.h"

#include "Runtime/CheatsSystem/CheatsSubsystem.h"
#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

void UCheatTeleportMenu::InitMenu()
{
	UCheatsSubsystem* tCheatsSubsystem = GetWorld()->GetSubsystem<UCheatsSubsystem>();
	if(tCheatsSubsystem == nullptr) return;
	CheatsSubsystem = tCheatsSubsystem;

	if(CheatsSubsystem->GetCheatTeleportPoints().Num() == 0) return;
	GetTeleportInfos(0);
}

void UCheatTeleportMenu::PrevTeleportInfos()
{
	if(CheatsSubsystem == nullptr) return;
	UpdateTeleportInfos(-1);
}

void UCheatTeleportMenu::NextTeleportInfos()
{
	if(CheatsSubsystem == nullptr) return;
	UpdateTeleportInfos(1);
}

void UCheatTeleportMenu::UpdateTeleportInfos(const int IndexDelta)
{
	if(CheatsSubsystem->GetCheatTeleportPoints().Num() == 0) return;
	
	int NewIndex = (CurrentTeleportInfo.Index + IndexDelta) % CheatsSubsystem->GetCheatTeleportPoints().Num();
	if(NewIndex < 0) NewIndex = CheatsSubsystem->GetCheatTeleportPoints().Num() - 1;

	GetTeleportInfos(NewIndex);
}

void UCheatTeleportMenu::GetTeleportInfos(const int Index)
{
	const ACheatTeleportPoint* CheatTeleportPoint = CheatsSubsystem->GetCheatTeleportPoints()[Index];
	if(CheatTeleportPoint == nullptr) return;
	
	const FName Name(CheatTeleportPoint->GetName());
	CurrentTeleportInfo.Index = Index;
	CurrentTeleportInfo.ShortName = Name;
}

void UCheatTeleportMenu::CheatTeleport() const
{
	if(CheatsSubsystem == nullptr) return;
	CheatsSubsystem->CheatTeleport(CurrentTeleportInfo.Index);
}
