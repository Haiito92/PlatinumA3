// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorWeatherPreset.h"
#include "SkyCreatorActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CoreMinimal.h"

#if WITH_EDITOR
void USkyCreatorWeatherPreset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	AActor* SkyCreator = NULL;

	UWorld* World = GEngine->GetWorldContexts()[0].World();
	if (World)
	{
		SkyCreator = UGameplayStatics::GetActorOfClass(World, ASkyCreator::StaticClass());
	}

	if (SkyCreator)
	{
		SkyCreator->PostEditChange();
	}
}
#endif

FSkyCreatorWeatherSettings USkyCreatorWeatherPreset::GetWeatherPresetSettings()
{
	FSkyCreatorWeatherSettings WeatherSettings;

	WeatherSettings.SkyAtmosphereSettings = SkyAtmosphereSettings;
	WeatherSettings.VolumetricCloudSettings = VolumetricCloudSettings;
	WeatherSettings.BackgroundCloudSettings = BackgroundCloudSettings;
	WeatherSettings.SkyLightSettings = SkyLightSettings;
	WeatherSettings.SunLightSettings = SunLightSettings;
	WeatherSettings.MoonLightSettings = MoonLightSettings;
	WeatherSettings.ExponentialHeightFogSettings = ExponentialHeightFogSettings;
	WeatherSettings.StarMapSettings = StarMapSettings;
	WeatherSettings.WeatherFXSettings = WeatherFXSettings;
	WeatherSettings.WeatherMaterialFXSettings = WeatherMaterialFXSettings;
	WeatherSettings.WindSettings = WindSettings;
	WeatherSettings.PostProcessSettings = PostProcessSettings;

	return WeatherSettings;
}