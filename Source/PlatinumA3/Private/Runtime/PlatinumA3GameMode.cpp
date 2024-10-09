// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlatinumA3/Public/Runtime/PlatinumA3GameMode.h"
#include "PlatinumA3/Public/Runtime/PlatinumA3Character.h"
#include "UObject/ConstructorHelpers.h"

APlatinumA3GameMode::APlatinumA3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
