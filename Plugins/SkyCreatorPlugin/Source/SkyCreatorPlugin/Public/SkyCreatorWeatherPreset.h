// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveLinearColor.h"
#include "SkyCreatorWeatherSettings.h"
#include "SkyCreatorWeatherPreset.generated.h"



UCLASS(BlueprintType)
class SKYCREATORPLUGIN_API USkyCreatorWeatherPreset : public UDataAsset
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Description", DisplayName = "Description", meta = (multiline = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Atmosphere", DisplayName = "Sky Atmosphere Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSkyAtmosphereSettings SkyAtmosphereSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Cloud", DisplayName = "Volumetric Cloud Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorVolumetricCloudSettings VolumetricCloudSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Background Clouds", DisplayName = "Background Cloud Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorBackgroundCloudSettings BackgroundCloudSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light", DisplayName = "Sky Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSkyLightSettings SkyLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Sun Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSunLightSettings SunLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Moon Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorMoonLightSettings MoonLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exponential Height Fog", DisplayName = "Exponential Height Fog Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorExponentialHeightFogSettings ExponentialHeightFogSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Star Map", DisplayName = "Star Map Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorStarMapSettings StarMapSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX", DisplayName = "Weather FX Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWeatherFXSettings WeatherFXSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather Material FX", DisplayName = "Weather Material FX Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWeatherMaterialFXSettings WeatherMaterialFXSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wind", DisplayName = "Wind Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWindSettings WindSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Post Process", DisplayName = "Post Process Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorPostProcessSettings PostProcessSettings;


	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather Preset")
	FSkyCreatorWeatherSettings GetWeatherPresetSettings();

};
