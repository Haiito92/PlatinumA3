// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorWeatherPresetFactory.h"
#include "SkyCreatorEditorPlugin.h"
#include "SkyCreatorWeatherPreset.h"

#define LOCTEXT_NAMESPACE "FSkyCreatorEditorPluginModule" 

USkyCreatorWeatherPresetFactory::USkyCreatorWeatherPresetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = USkyCreatorWeatherPreset::StaticClass();
}

UObject* USkyCreatorWeatherPresetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	USkyCreatorWeatherPreset* SkyCreatorWeatherPreset = NewObject<USkyCreatorWeatherPreset>(InParent, Class, Name, Flags);
	return SkyCreatorWeatherPreset;
}

FText USkyCreatorWeatherPresetFactory::GetDisplayName() const
{
	return LOCTEXT("SkyCreatorWeatherPresetText", "Sky Creator Weather Preset");
}

FString USkyCreatorWeatherPresetFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewWeatherPreset"));
}
