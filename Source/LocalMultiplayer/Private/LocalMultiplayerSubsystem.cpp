// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	FString OutError;

	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	const int NbMaxPlayers = LocalMultiplayerSettings->NbMaxGamepads + LocalMultiplayerSettings->GetNbKeyboardProfiles();
	for (int i = 0; i < NbMaxPlayers; ++i)
	{
		ULocalPlayer* LocalPlayer = GetGameInstance()->CreateLocalPlayer(i, OutError, true);
	}

	//Parcourir TMap et assigner les bons IMC
	for (const TTuple<int,int> KeyboardProfileIndexPlayerIndexPair : PlayerIndexFromKeyboardProfileIndex)
	{
		const int KeyboardProfileIndex = KeyboardProfileIndexPlayerIndexPair.Key;
		const int PlayerIndex = KeyboardProfileIndexPlayerIndexPair.Value;
		AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, MappingType);
	}

	for (const TTuple<int,int> GamepadProfilePlayerIndexPair : PlayerIndexFromGamepadProfileIndex)
	{
		const int PlayerIndex = GamepadProfilePlayerIndexPair.Value;

		AssignGamepadInputMapping(PlayerIndex, MappingType);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if(PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	
	UEnhancedInputLocalPlayerSubsystem * InputSubsystem = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex)->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	InputSubsystem->AddMappingContext(LocalMultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType),0, ModifyContextOptions);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	
	UEnhancedInputLocalPlayerSubsystem * InputSubsystem = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex)->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	InputSubsystem->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType),0, ModifyContextOptions);
}
