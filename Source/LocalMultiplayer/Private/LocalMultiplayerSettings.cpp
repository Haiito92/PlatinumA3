// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalMultiplayerSettings.h"
#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	const UInputMappingContext* IMC = GetIMCFromType(MappingType);
	if (IMC == nullptr) return false;

	for (FEnhancedActionKeyMapping Element : IMC->GetMappings())
	{
		if(Element.Key == Key) return true;
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	default: return nullptr;
	}
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); ++i)
	{
		for (FEnhancedActionKeyMapping Element : KeyboardProfilesData[i].GetIMCFromType(MappingType)->GetMappings())
		{
			if(Element.Key == Key) return i;
		}
	}
	return -1;
}
