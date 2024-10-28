// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorEditorPlugin.h"

#include "IPlacementModeModule.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "ThumbnailRendering/ClassThumbnailRenderer.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "ActorFactories/ActorFactory.h"
#include "SkyCreatorActor.h"
#include "SkyCreatorWeatherPreset.h"
#include "SkyCreatorActorFactory.h"
#include "Runtime/Launch/Resources/Version.h"

//#define LOCTEXT_NAMESPACE "FSkyCreatorEditorPluginModule"

void FSkyCreatorEditorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	FPlacementCategoryInfo Info = *PlacementModeModule.GetRegisteredPlacementCategory(FBuiltInPlacementCategories::Lights());

	FPlaceableItem* SkyCreatorPlacement = new FPlaceableItem(
		*USkyCreatorActorFactory::StaticClass(),
		FAssetData(ASkyCreator::StaticClass(), false),
		FName("ClassThumbnail.SkyCreator"),
#if ENGINE_MAJOR_VERSION == 5
		FName("ClassIcon.SkyCreator"),
#endif
		TOptional<FLinearColor>(),
		TOptional<int32>(),
		NSLOCTEXT("PlacementMode", "Sky Creator", "Sky Creator")
	);

	IPlacementModeModule::Get().RegisterPlaceableItem(Info.UniqueHandle, MakeShareable(SkyCreatorPlacement));

	StyleSet = MakeShareable(new FSlateStyleSet("SkyCreatorStyle"));

	//Content path of this plugin
	FString ContentDir = IPluginManager::Get().FindPlugin("SkyCreatorPlugin")->GetBaseDir();

	StyleSet->SetContentRoot(ContentDir);

	//Create a brush from the icon
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/Icon128"), TEXT(".png")), FVector2D(128.f, 128.f));

	FSlateImageBrush* SkyCreatorClassThumbnail = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/SkyCreator_Icon"), TEXT(".png")), FVector2D(64.f, 64.f));
	//	FSlateImageBrush* SkyCreatorClassThumbnail = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/SkyCreator_Icon"), TEXT(".png")), FVector2D(64.f, 64.f), FLinearColor(0, 1, 1, 1), ESlateBrushTileType::NoTile, ESlateBrushImageType::FullColor);
	FSlateImageBrush* SkyCreatorClassIcon = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/SkyCreator_Icon"), TEXT(".png")), FVector2D(16.f, 16.f));

	FSlateImageBrush* SkyCreatorWeatherPresetClassThumbnail = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/SkyCreator_WeatherPresetIcon"), TEXT(".png")), FVector2D(128.f, 128.f));
	FSlateImageBrush* SkyCreatorWeatherPresetClassIcon = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/SkyCreator_WeatherPresetIcon"), TEXT(".png")), FVector2D(16.f, 16.f));

	if (ThumbnailBrush && SkyCreatorClassThumbnail && SkyCreatorClassIcon && SkyCreatorWeatherPresetClassThumbnail && SkyCreatorWeatherPresetClassIcon)
	{
		//In order to bind the thumbnail to our class we need to type ClassThumbnail.X where X is the name of the C++ class of the asset
		StyleSet->Set("ClassThumbnail.SkyCreatorWeatherPreset", ThumbnailBrush);
//		StyleSet->Set("ClassIcon.SkyCreatorWeatherPreset", ThumbnailBrush);

		StyleSet->Set("ClassThumbnail.SkyCreator", SkyCreatorClassThumbnail);
//		StyleSet->Set("ClassIcon.SkyCreator", SkyCreatorClassIcon);

		StyleSet->Set("ClassThumbnail.SkyCreatorWeatherPreset", SkyCreatorWeatherPresetClassThumbnail);
		StyleSet->Set("ClassIcon.SkyCreatorWeatherPreset", SkyCreatorWeatherPresetClassIcon);

		//Register the created style
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}

	
}

void FSkyCreatorEditorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

		// Unregister editor category
	if (IPlacementModeModule::IsAvailable())
	{
//		IPlacementModeModule::Get().UnregisterPlaceableItem(SkyCreatorPlacement);
	}

	//Unregister the style
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSkyCreatorEditorPluginModule, SkyCreatorEditorPlugin)