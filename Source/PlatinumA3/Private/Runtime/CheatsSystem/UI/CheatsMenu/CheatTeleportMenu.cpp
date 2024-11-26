// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/UI/CheatsMenu/CheatTeleportMenu.h"

#include "Runtime/CheatsSystem/CheatsSubsystem.h"
#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

void UCheatTeleportMenu::GetTeleportInfos(int Index)
{
	const UCheatsSubsystem* CheatsSubsystem = GetWorld()->GetSubsystem<UCheatsSubsystem>();
	if(CheatsSubsystem == nullptr) return;

	const ACheatTeleportPoint* CheatTeleportPoint = CheatsSubsystem->GetCheatTeleportPoints()[Index];
	if(CheatTeleportPoint == nullptr) return;
	FName Name(CheatTeleportPoint->GetName());
	CurrentTeleportInfo.ShortName = Name;
}
