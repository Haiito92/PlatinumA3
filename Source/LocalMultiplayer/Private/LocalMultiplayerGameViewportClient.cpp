// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();

	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	MaxSplitscreenPlayers = LocalMultiplayerSettings->GetNbKeyboardProfiles() + LocalMultiplayerSettings->NbMaxGamepads;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	int PlayerIndex = -1;
	if (!EventArgs.IsGamepad())
	{
		const int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(
			EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardProfileIndex >= 0)
		{
			PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
			if (PlayerIndex < 0)
			{
				PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
				LocalMultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex,
				                                                 ELocalMultiplayerInputMappingType::InGame);

				//const ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
				//return LocalPlayer->PlayerController->InputKey(FInputKeyParams(EventArgs.Key, EventArgs.Event, static_cast<double>(EventArgs.AmountDepressed), EventArgs.IsGamepad(), EventArgs.InputDevice));
			}
		}
		//return Super::InputKey(EventArgs);
	}
	else
	{
		const int DeviceID = EventArgs.ControllerId;

		PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID) < 0;
		if (PlayerIndex < 0)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
			LocalMultiplayerSubsystem->
				AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);


			//return LocalPlayer->PlayerController->InputKey(FInputKeyParams(EventArgs.Key, EventArgs.Event, static_cast<double>(EventArgs.AmountDepressed), EventArgs.IsGamepad(), EventArgs.InputDevice));
		}
	}

	if (PlayerIndex > -1)
	{
		const ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
		return LocalPlayer->PlayerController->InputKey(FInputKeyParams(EventArgs.Key, EventArgs.Event,
		                                                               EventArgs.AmountDepressed, EventArgs.IsGamepad(),
		                                                               EventArgs.InputDevice));
	}

	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	int PlayerIndex = -1;
	if (!bGamepad)
	{
		const int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(
			Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardProfileIndex >= 0)
		{
			PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
			if (PlayerIndex < 0)
			{
				PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
				LocalMultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex,
																 ELocalMultiplayerInputMappingType::InGame);
			}
		}
	}
	else
	{
		const int DeviceID = InputDevice.GetId();

		PlayerIndex =LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
		if (PlayerIndex < 0)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
			LocalMultiplayerSubsystem->
				AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}

	if (PlayerIndex > -1)
	{
		const ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
		return LocalPlayer->PlayerController->InputKey(FInputKeyParams(
			Key,
			Delta,
			DeltaTime,
			NumSamples,
			bGamepad
		));
	}
	
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}