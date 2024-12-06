// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignPlayerStart.h"

TSubclassOf<AWoolStateCharacter> ACampaignPlayerStart::GetClassToSpawn() const
{
	return ClassToSpawn;
}

void ACampaignPlayerStart::SetClassToSpawn(TSubclassOf<AWoolStateCharacter> InClassToSpawn)
{
	ClassToSpawn = InClassToSpawn;
}

EWoolStateClassID ACampaignPlayerStart::GetWoolStateClassID() const
{
	return WoolStateClassID;
}

void ACampaignPlayerStart::SetWoolStateClassID(EWoolStateClassID InClassID)
{
	WoolStateClassID = InClassID;
}
