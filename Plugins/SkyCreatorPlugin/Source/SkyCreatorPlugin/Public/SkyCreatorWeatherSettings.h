// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkyCreatorWeatherSettings.generated.h"

USTRUCT(BlueprintType)
struct FSkyCreatorSkyAtmosphereSettings
{
	GENERATED_BODY()

	/** Rayleigh scattering coefficient scale.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Rayleigh", DisplayName = "Rayleigh Scattering Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0", SliderExponent = 4.0))
	float RayleighScatteringScale = 0.04f;

	/** The Rayleigh scattering coefficients resulting from molecules in the air at an altitude of 0 kilometer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Rayleigh", DisplayName = "Rayleigh Scattering", meta = (HideAlphaChannel))
	FLinearColor RayleighScattering = FLinearColor(0.118f, 0.28f, 1.000000f);

	/** The altitude in kilometer at which Rayleigh scattering effect is reduced to 40%.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Rayleigh", DisplayName = "Rayleigh Exponential Distribution", meta = (ClampMin = "0.001", UIMin = "0.01", UIMax = "20.0", SliderExponent = 5.0))
	float RayleighExponentialDistribution = 8.0f;


	/** Mie scattering coefficient scale.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Scattering Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5.0", SliderExponent = 4.0))
	float MieScatteringScale = 0.002f;

	/** 
	* The Mie scattering coefficients resulting from particles in the air at an altitude of 0 kilometer. 
	* As it becomes higher, light will be scattered more. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Scattering", meta = (HideAlphaChannel))
	FLinearColor MieScattering = FLinearColor(1, 1, 1);

	/** Mie absorption coefficient scale.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Absorption Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5.0", SliderExponent = 4.0))
	float MieAbsorptionScale = 0.04f;

	/** 
	* The Mie absorption coefficients resulting from particles in the air at an altitude of 0 kilometer. 
	* As it becomes higher, light will be absorbed more. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Absorption", meta = (HideAlphaChannel))
	FLinearColor MieAbsorption = FLinearColor(1, 1, 1);

	/** 
	* A value of 0 mean light is uniformly scattered. 
	* A value closer to 1 means lights will scatter more forward, resulting in halos around light sources. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Anisotropy", meta = (ClampMin = "0.0", ClampMax = "0.999", UIMin = "0.0", UIMax = "0.999"))
	float MieAnisotropy = 0.72f;

	/** The altitude in kilometer at which Mie effects are reduced to 40%.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Mie", DisplayName = "Mie Exponential Distribution", meta = (ClampMin = "0.01", UIMin = "0.01", UIMax = "10.0", SliderExponent = 5.0))
	float MieExponentialDistribution = 1.2f;


	/**
	* Absorption coefficients for another atmosphere layer.
	* Density increase from 0 to 1 between 10 to 25km and decreases from 1 to 0 between 25 to 40km.
	* This approximates ozone molecules distribution in the Earth atmosphere.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Absorption", DisplayName = "Absorption Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "0.2", SliderExponent = 3.0))
	float AbsorptionScale = 0.004f;

	/**
	* Absorption coefficients for another atmosphere layer.
	* Density increase from 0 to 1 between 10 to 25km and decreases from 1 to 0 between 25 to 40km.
	* The default values represents ozone molecules absorption in the Earth atmosphere.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Absorption", DisplayName = "Absorption", meta = (HideAlphaChannel))
	FLinearColor Absorption = FLinearColor(0.9f, 1.0f, 0.36f);


	/** Scales the luminance of pixels representing the sky, i.e. not belonging to any surface. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Art Direction", DisplayName = "Sky Luminance Factor", meta = (HideAlphaChannel))
	FLinearColor SkyLuminanceFactor = FLinearColor(1, 1, 1);


	/** 
	* The ground albedo that will tint the astmophere when the sun light will bounce on it.
	* Only taken into account when MultiScattering > 0.0. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Art Direction", DisplayName = "Ground Albedo", meta = (HideAlphaChannel))
	FLinearColor GroundAlbedo = FLinearColor(0.65f, 0.65f, 0.65f);

	/**
	* Scale the sky and atmosphere lights contribution to the height fog when
	* SupportSkyAtmosphereAffectsHeightFog project setting is true.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere|Art Direction", DisplayName = "Height Fog Contribution", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float HeightFogContribution = 1.0f;
};
	/** Description. */
USTRUCT(BlueprintType)
struct FSkyCreatorVolumetricCloudSettings
{
	GENERATED_BODY()

	/** Coverage of the Stratus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratus Coverage", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float StratusCoverage = 0.0f;

	/** Coverage variation of the Stratus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratus Coverage Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StratusCoverageVariation = 0.5f;

	/** Height variation of the Stratus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratus Height Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float StratusHeightVariation = 0.5f;

	/** Coverage of the Stratocumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratocumulus Coverage", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float StratocumulusCoverage = 0.75f;

	/** Coverage variation of the Stratocumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratocumulus Coverage Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StratocumulusCoverageVariation = 0.5f;

	/** Height variation of the Stratocumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Stratocumulus Height Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float StratocumulusHeightVariation = 0.5f;

	/** Coverage of the Cumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulus Coverage", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float CumulusCoverage = 0.75f;

	/** Coverage variation of the Cumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulus Coverage Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CumulusCoverageVariation = 0.5f;

	/** Height variation of the Cumulus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulus Height Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float CumulusHeightVariation = 0.5f;

	/** Coverage of the Cumulonimbus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulonimbus Coverage", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CumulonimbusCoverage = 0.0f;

	/** Anvil factor of the Cumulonimbus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulonimbus Anvil", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CumulonimbusAnvil = 0.0f;

	/** Height variation of the Cumulonimbus cloud layer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Cumulonimbus Height Variation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float CumulonimbusHeightVariation = 0.5f;


	/** Density value of the bottom level of the Volumetric Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Parameters", DisplayName = "Density Bottom", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float DensityBottom = 0.001f;

	/** Density value of the middle level of the Volumetric Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Parameters", DisplayName = "Density Middle", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float DensityMiddle = 0.05f;

	/** Density value of the top level of the Volumetric Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Parameters", DisplayName = "Density Top", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float DensityTop = 0.1f;


	/** Albedo color of Volumetric Cloudss. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Albedo")
	FLinearColor Albedo = FLinearColor(1, 1, 1);

	/**
	* The ground albedo used to light the cloud from below with respect to the sun light and sky atmosphere.
	* This is only used by the cloud material when the Ground Contribution is enabled from the Quality settings of Volumetric Clouds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Ground Albedo", meta = (HideAlphaChannel))
	FLinearColor GroundAlbedo = FLinearColor(0.25f, 0.25f, 0.25f);

	/** Beer's Powder Intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Beer's Powder Intensity", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BeersPowderIntensity = 0.25f;

	/** Beer's Powder Depth. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Beer's Powder Depth", meta = (ClampMin = "0.0", UIMin = "10.0", UIMax = "200.0"))
	float BeersPowderDepth = 100.0f;

	/** Bottom Occlusion amount of the volumetric clouds. Affects only Cumulus and Cumulonimbus layers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Bottom Occlusion", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BottomOcclusion = 0.0f;

	/** Normalized height of the Bottom Occlusion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Bottom Occlusion Height", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BottomOcclusionHeight = 0.25f;

	/** Emissive color of Volumetric Cloudss. Better to keep this value low. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Night Emissive")
	FLinearColor NightEmissive = FLinearColor(0.010725f, 0.015880f, 0.025000f); // FLinearColor(0.010725f, 0.015880f, 0.025000f);

	/** Phase function describing how much forward (G < 0) of backward (G > 0) light scatter around. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Phase G", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float PhaseG = 0.6f;

	/** Second phase function describing how much forward (G < 0) of backward (G > 0) light scatter around. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Phase G2", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float PhaseG2 = -0.4f;

	/** Lerp factor when blending the two phase functions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Phase Blend", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PhaseBlend = 0.5f;

	/**
	* Multi-scattering approximation: represents how much contribution 
	* each successive octave will add. Evaluatead per pixel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Multi Scattering Contribution", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MultiScatteringContribution = 0.5f;

	/**
	* Multi-scattering approximation: represents how much occlussion
	* will be reduced for each successive octave. Evaluatead per pixel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Multi Scattering Occlusion", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MultiScatteringOcclusion = 0.1f;

	/**
	* Multi-scattering approximation: represents how much the phase 
	* will become isotropic for each successive octave. Evaluatead per pixel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Lighting", DisplayName = "Multi Scattering Eccentricity", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MultiScatteringEccentricity = 0.5f;


	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Shape Intensity A", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float NoiseShapeIntensityA = 0.5f;

	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Shape Intensity B", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float NoiseShapeIntensityB = 0.625f;

	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Shape Intensity C", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float NoiseShapeIntensityC = 0.25f;

	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Shape Intensity D", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float NoiseShapeIntensityD = 0.125f;


	/** Desc. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Detail Intensity A", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
//	float NoiseDetailIntensityA = 0.625f;

	/** Desc. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Detail Intensity B", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
//	float NoiseDetailIntensityB = 0.25f;

	/** Desc. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Noise Detail Intensity C", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
//	float NoiseDetailIntensityC = 0.125f;

	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds|Noise", DisplayName = "Turbulence Intensity", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float TurbulenceIntensity = 0.25f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorBackgroundCloudSettings
{
	GENERATED_BODY()

	/** Overall intensity of Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float BackgroundCloudsIntensity = 1.0f;

	/** Color Tint of Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Color Tint")
	FLinearColor BackgroundCloudsColorTint = FLinearColor(1, 1, 1);

	/** Amount of 'Layer A' in Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Layer A", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BackgroundCloudsLayerA = 0.5f;

	/** Amount of 'Layer B' in Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Layer B", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BackgroundCloudsLayerB = 0.5f;

	/** Amount of 'Layer C' in Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Layer C", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BackgroundCloudsLayerC = 0.5f;

	/** Amount of Lightning Phase in Background Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Lightning Phase", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "0.999"))
	float BackgroundCloudsLightningPhase = 0.8f;
};


USTRUCT(BlueprintType)
struct FSkyCreatorSkyLightSettings
{
	GENERATED_BODY()

	/** Total energy that the light emits. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Intensity", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Intensity = 1.0f;

	/** Total energy that the light emits. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Day Intensity", meta = (ClampMin = "0.0", UIMin = "0.0"))
	//float DayIntensity = 1.0f;

	/** Energy that the light emits in the nights. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Night Intensity", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float NightIntensity = 4.0f;

	/**
	* Filter color of the light. 
	* Note that this can change the light's effective intensity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Light Color", meta = (HideAlphaChannel))
	FLinearColor LightColor = FLinearColor(1.0f, 1.0f, 1.0f);

	/** Lower Hemisphere Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Lower Hemisphere Color", meta = (HideAlphaChannel))
	FLinearColor LowerHemisphereColor = FLinearColor(0.0006, 0.0007, 0.001, 1);

	/** The strength of the ambient occlusion, higher value will block more light. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", meta = (UIMin = "0", UIMax = "1", ClampMin = "0", SliderExponent = 1.0))
	float CloudAmbientOcclusionStrength = 0.25f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorSunLightSettings
{
	GENERATED_BODY()

	/** Maximum illumination from the light in lux. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light", DisplayName = "Intensity", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Intensity = 10.0f;

	/**
	* Filter color of the light.
	* Note that this can change the light's effective intensity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light", DisplayName = "Light Color", meta = (HideAlphaChannel))
	FLinearColor LightColor = FLinearColor(1, 1, 0.9);

	/**
	* Color temperature in Kelvin of the blackbody illuminant.
	* White (D65) is 6500K.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light", DisplayName = "Temperature", meta = (ClampMin = "1700.0", ClampMax = "12000.0", UIMin = "1700.0", UIMax = "12000.0"))
	float Temperature = 5500.0f;

	/**
	* Intensity of the volumetric scattering from sun.
	* This scales Intensity and Light Color.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light", DisplayName = "Volumetric Scattering Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float VolumetricScatteringIntensity = 1.0f;

	/** A color multiplied with the sun disk luminance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Atmosphere Disk Color Scale", meta = (HideAlphaChannel))
	FLinearColor AtmosphereDiskColorScale = FLinearColor(1, 1, 1);

	/** Scales the lights contribution when scattered in cloud participating media. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Scattered Luminance Scale", meta = (HideAlphaChannel))
	FLinearColor CloudScatteredLuminanceScale = FLinearColor(1, 1, 1);

	/** The overall strength of the cloud shadow, higher value will block more light. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowStrength = 1.0f;

	/** The strength of the shadow on atmosphere. Disabled when 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow on Atmosphere Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowOnAtmosphereStrength = 0.5f;

	/** The strength of the shadow on opaque and transparent meshes. Disabled when 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow on Surface Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowOnSurfaceStrength = 0.9f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorMoonLightSettings
{
	GENERATED_BODY()

	/** Maximum illumination from the light in lux. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Intensity", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Intensity = 0.04f;

	/**
	* Filter color of the light.
	* Note that this can change the light's effective intensity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Light Color", meta = (HideAlphaChannel))
	FLinearColor LightColor = FLinearColor(0.48f, 0.74f, 1.0f);

	/**
	* Color temperature in Kelvin of the blackbody illuminant.
	* White (D65) is 6500K.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Temperature", meta = (ClampMin = "1700.0", ClampMax = "12000.0", UIMin = "1700.0", UIMax = "12000.0"))
	float Temperature = 8500.0f;

	/**
	* Intensity of the volumetric scattering from moon.
	* This scales Intensity and Light Color.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Volumetric Scattering Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float VolumetricScatteringIntensity = 1.0f;

	/** A color multiplied with the sun disk luminance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Atmosphere Disk Color Scale", meta = (HideAlphaChannel))
	FLinearColor AtmosphereDiskColorScale = FLinearColor(1.0f, 0.88f, 0.4f);

	/** Scales the lights contribution when scattered in cloud participating media. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Scattered Luminance Scale", meta = (HideAlphaChannel))
	FLinearColor CloudScatteredLuminanceScale = FLinearColor(1, 1, 1);

	/** The overall strength of the cloud shadow, higher value will block more light. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowStrength = 1.0f;

	/** The strength of the shadow on atmosphere. Disabled when 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow on Atmosphere Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowOnAtmosphereStrength = 0.5f;

	/** The strength of the shadow on opaque and transparent meshes. Disabled when 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow on Surface Strength", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudShadowOnSurfaceStrength = 0.9f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorExponentialHeightFogSettings
{
	GENERATED_BODY()

	/** Global density factor of fog. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Fog", DisplayName = "Fog Density", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float FogDensity = 0.002f;

	/**
	* Height density factor, controls how the density increases as height decreases.
	* Smaller values make the visible transition larger.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Fog", DisplayName = "Fog Height Falloff", meta = (ClampMin = "0.001", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float FogHeightFalloff = 0.06f;

	/** Fog Inscattering Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Fog", DisplayName = "Fog Inscattering Color", meta = (HideAlphaChannel))
	FLinearColor FogInscatteringColor = FLinearColor::Black; // FLinearColor(0.001113, 0.001528, 0.002);

	/** Distance from the camera that the fog will start. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Fog", DisplayName = "Fog Start Distance", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5000.0"))
	float FogStartDistance = 0.0f;


	/** Global density factor of secondary fog. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Second Fog", DisplayName = "Second Fog Density", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float SecondFogDensity = 0.0f;

	/**
	* Height density factor, controls how the density increases as height decreases.
	* Smaller values make the visible transition larger.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Second Fog", DisplayName = "Second Fog Height Falloff", meta = (ClampMin = "0.001", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float SecondFogHeightFalloff = 0.6f;


	/**
	* Controls the size of the directional inscattering cone, which is used to approximate inscattering from a directional light.
	* Note: there must be a directional light with bUsedAsAtmosphereSunLight enabled for DirectionalInscattering to be used.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Directional Inscattering", DisplayName = "Directional Inscattering Exponent", meta = (ClampMin = "2.0", ClampMax = "64.0", UIMin = "2.0", UIMax = "64.0"))
	float DirectionalInscatteringExponent = 8.0f;

	/**
	* Controls the start distance from the viewer of the directional inscattering, which is used to approximate inscattering from a directional light.
	* Note: there must be a directional light with bUsedAsAtmosphereSunLight enabled for DirectionalInscattering to be used.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Directional Inscattering", DisplayName = "Directional Inscattering Start Distance", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DirectionalInscatteringStartDistance = 2000.0f;

	/**
	* Controls the color of the directional inscattering, which is used to approximate inscattering from a directional light.
	* Note: there must be a directional light with bUsedAsAtmosphereSunLight enabled for DirectionalInscattering to be used.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Directional Inscattering", DisplayName = "Directional Inscattering Color", meta = (HideAlphaChannel))
	FLinearColor DirectionalInscatteringColor = FLinearColor(0, 0, 0);

	/**
	* Controls the scattering phase function - how much incoming light scatters in various directions.
	* A distribution value of 0 scatters equally in all directions, while .9 scatters predominantly in the light direction.
	* In order to have visible volumetric fog light shafts from the side, the distribution will need to be closer to 0.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Volumetric Fog", DisplayName = "Volumetric Fog Scattering Distribution", meta = (ClampMin = "-0.9", ClampMax = "0.9", UIMin = "-0.9", UIMax = "0.9"))
	float VolumetricFogScatteringDistribution = 0.2f;

	/**
	* The height fog particle reflectiveness used by volumetric fog.
	* Water particles in air have an albedo near white, while dust has slightly darker value.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Volumetric Fog", DisplayName = "Volumetric Fog Albedo", meta = (HideAlphaChannel))
	FLinearColor VolumetricFogAlbedo = FLinearColor(1, 1, 1);

	/**
	* Light emitted by height fog.  This is a density so more light is emitted the further you are looking through the fog.
	* In most cases skylight is a better choice, however right now volumetric fog does not support precomputed lighting,
	* So stationary skylights are unshadowed and static skylights don't affect volumetric fog at all.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Volumetric Fog", DisplayName = "Volumetric Fog Emissive", meta = (HideAlphaChannel))
	FLinearColor VolumetricFogEmissive = FLinearColor(0, 0, 0);

	/**
	* Scales the height fog particle extinction amount used by volumetric fog.
	* Values larger than 1 cause fog particles everywhere absorb more light.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog|Volumetric Fog", DisplayName = "Volumetric Fog Extinction Scale", meta = (ClampMin = "0.1", ClampMax = "100.0", UIMin = "0.1", UIMax = "10.0"))
	float VolumetricFogExtinctionScale = 1.0f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorStarMapSettings
{
	GENERATED_BODY()

	/** Overall brightness of the Star Map. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float StarMapIntensity = 0.25f;

	/** Overall color tint of the Star Map. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Color Tint", meta = (HideAlphaChannel))
	FLinearColor StarMapColorTint = FLinearColor(1, 1, 1);

	/** Intensity of twinkling stars. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Twinkle Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StarMapTwinkleIntensity = 1.0f;

	/** Saturation of twinkling stars. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Twinkle Saturation", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StarMapTwinkleSaturation = 0.25f;

	/** Twinkle speed of stars. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Twinkle Speed", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StarMapTwinkleSpeed = 1.0f;

	/**
	* Indicates how strong the star map can be faded into horizon.
	* Lower values cause more stars appear at horizon.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Horizon Threshold", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "16.0"))
	float StarMapHorizonThreshold = 8.0f;

	/**
	* Indicates how strong the star map can be faded into atmosphere.
	* Lower values cause more stars appear at brighter points in atmosphere, e.g. at daytime.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Atmosphere Threshold", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "16.0"))
	float StarMapAtmosphereThreshold = 4.0f;

	/** Color tint of the horizon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Night Horizon Color", meta = (HideAlphaChannel))
	FLinearColor NightHorizonColor = FLinearColor(0.019600f, 0.027609f, 0.040000f); // FLinearColor(0.039463f, 0.050910f, 0.072917f)

	/** Color tint of the zenith. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Night Zenith Color", meta = (HideAlphaChannel))
	FLinearColor NightZenithColor = FLinearColor(0.001384f, 0.002343f, 0.004000f); // FLinearColor(0.003460f, 0.005858f, 0.01f)
};

USTRUCT(BlueprintType)
struct FSkyCreatorWeatherFXSettings
{
	GENERATED_BODY()

	/** Rain Precipitation Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Amount", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float RainAmount = 0.0f;

	/** Rain Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Color")
	FLinearColor RainColor = FLinearColor(0.5, 0.5, 0.5, 0.5);

	/** Gravity of a biggest sized raindrop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Gravity")
	float RainGravity = -1000.0f;

	/** Minimum lifetime of a single raindrop. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Lifetime Min")
	float RainLifetimeMin = 0.3f;

	/** Maximum lifetime of a single raindrop. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Lifetime Max")
	float RainLifetimeMax = 0.6f;

	/** Minimum size of a single raindrop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Size Min")
	float RainSizeMin = 0.1f;

	/** Maximum size of a single raindrop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain", DisplayName = "Rain Size Max")
	float RainSizeMax = 0.8f;


	/** Minimum lifetime of a single rain splash. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Lifetime Min")
	float RainSplashLifetimeMin = 0.2f;

	/** Maximum lifetime of a single rain splash. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Lifetime Max")
	float RainSplashLifetimeMax = 0.3f;

	/** Minimum size of a rain splash. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Size Min")
	FVector2D RainSplashSizeMin = FVector2D(16.0f, 16.0f);

	/** Maximum size of a rain splash. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Size Max")
	FVector2D RainSplashSizeMax = FVector2D(24.0f, 24.0f);


	/** Snow Precipitation Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Amount", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowAmount = 0.0f;

	/** Snow Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Color")
	FLinearColor SnowColor = FLinearColor(1, 1, 1, 1);

	/** Gravity of a biggest sized snowflake. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Gravity")
	float SnowGravity = -100.0f;

	/** Snow turbulence. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Turbulence", meta = (UIMin = "0.0", UIMax = "1000.0"))
	float SnowTurbulence = 100.0f;

	/** Minimum lifetime of a single snowflake. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Lifetime Min")
	float SnowLifetimeMin = 1.0f;

	/** Maximum lifetime of a single snowflake. Can affect the performance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Lifetime Max")
	float SnowLifetimeMax = 2.0f;

	/** Minimum size of a single snowflake. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Size Min", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "5.0"))
	float SnowSizeMin = 1.0f;

	/** Maximum size of a single snowflake. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Snow", DisplayName = "Snow Size Max", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "5.0"))
	float SnowSizeMax = 2.0f;


	/** Whether to enable Lightnings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Enable Lightnings")
	bool EnableLightnings = false;

	/** Minimum interval to randomly spawn Lightning in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Lightning Spawn Interval Min")
	float LightningSpawnIntervalMin = 1.0f;

	/** Maximum interval to randomly spawn Lightning in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Lightning Spawn Interval Max")
	float LightningSpawnIntervalMax = 10.0f;

	/** Minimum Lightning color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Lightning Color Min")
	FLinearColor LightningColorMin = FLinearColor(1, 1, 1, 1);

	/** Maximum Lightning color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Lightning Color Max")
	FLinearColor LightningColorMax = FLinearColor(1, 1, 1, 1);

	/** Lightning lifetime in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings", DisplayName = "Lightning Lifetime")
	float LightningLifetime = 0.25f;


	/** Chance of spawning a Lightning Bolt when spawning a Lightning. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Spawn Chance", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LightningBoltSpawnChance = 50.0f;

	/** Minimum width of a Lightning Bolt. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Width Min")
	float LightningBoltWidthMin = 0.004f;

	/** Maximum width of a Lightning Bolt. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Width Max")
	float LightningBoltWidthMax = 0.008f;

	/** Minimum length of a Lightning Bolt (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Length Min")
	float LightningBoltLengthMin = 2.0f;

	/** Maximum length of a Lightning Bolt (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Length Max")
	float LightningBoltLengthMax = 4.0f;

	/** Frequency of a Lightning Bolt curvature. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Curve Frequency")
	float LightningBoltCurveFrequency = 0.5f;

	/** Minimum strength of a Lightning Bolt curvature. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Curve Strength Min")
	float LightningBoltCurveStrengthMin = 0.02f;

	/** Maximum strength of a Lightning Bolt curvature. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Curve Strength Max")
	float LightningBoltCurveStrengthMax = 0.08f;

	/** Jitter strength of a Lightning Bolt. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Bolt", DisplayName = "Lightning Bolt Jitter Strength")
	float LightningBoltJitterStrength = 0.0025f;


	/** Chance of spawning a Lightning Branch when spawning a Lightning Bolt. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Branches", DisplayName = "Lightning Branch Spawn Chance", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LightningBranchSpawnChance = 10.0f;

	/** Minimum scale of a Lightning Branch. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Branches", DisplayName = "Lightning Branch Scale Min", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LightningBranchScaleMin = 0.0004f;

	/** Maximum scale of a Lightning Branch. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Lightnings|Lightning Branches", DisplayName = "Lightning Branch Scale Max", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LightningBranchScaleMax = 0.0008f;


	/** Rainbow Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Amount", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float RainbowAmount = 0.0f;

	/** Rainbow Tint Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Color")
	FLinearColor RainbowColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

	/** Radius of the Rainbow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Radius", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RainbowRadius = 10000.0f;

	/** Thickness of the Rainbow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Thickness", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RainbowThickness = 800.0f;

	/** Secondary Rainbow Tint Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Secondary Rainbow Color")
	FLinearColor SecondaryRainbowColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.0f);

	/** Radius of the Secondary Rainbow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Secondary Rainbow Radius", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SecondaryRainbowRadius = 14000.0f;

	/** Thickness of the Secondary Rainbow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX|Rainbow", DisplayName = "Secondary Rainbow Thickness", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SecondaryRainbowThickness = 1600.0f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorWeatherMaterialFXSettings
{
	GENERATED_BODY()

	/** Wetness Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Wetness", DisplayName = "Wetness Amount", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WetnessAmount = 0.0f;

	/** Wetness Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Wetness", DisplayName = "Wetness Color", meta = (HideAlphaChannel))
	FLinearColor WetnessColor = FLinearColor(0.5, 0.5, 0.5, 1);

	/** Puddles Coverage Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Puddles", DisplayName = "Puddles Amount", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PuddlesAmount = 0.0f;

	/** Puddles Coverage Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Puddles", DisplayName = "Puddles Color", meta = (HideAlphaChannel))
	FLinearColor PuddlesColor = FLinearColor(0.3, 0.3, 0.3, 1);


	/** Rain Ripples Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Amount", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float RainRipplesAmount = 0.0f;

	/** Rain Ripples normap map intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float RainRipplesIntensity = 0.1f;

	/** Min Force Radius to write by the Rain Ripples Solver. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Force Min Radius", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float RainRipplesForceMinRadius = 0.5f;

	/** Max Force Radius to write by the Rain Ripples Solver. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Force Max Radius", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float RainRipplesForceMaxRadius = 1.0f;

	/** Number of Rain Ripples Rings to propagate by a single raindrop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Rings Number", meta = (ClampMin = "0.0", UIMin = "1.0", UIMax = "4.0"))
	float RainRipplesRingsNumber = 2.0f;

	/** Delay of Rain Ripples Rings propagation. Number represents Rain Ripples Solver frames to skip. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Rings Delay", meta = (ClampMin = "0.0", UIMin = "1.0", UIMax = "10.0"))
	float RainRipplesRingsDelay = 8.0f;

	/** Damping of Rain Ripples. Higher values are fading Rain Ripples faster. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Damping", meta = (ClampMin = "0.001", UIMin = "0.001", UIMax = "1.0"))
	float RainRipplesDamping = 0.05f;

	/** Wind Ripples normap map intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Wind Ripples Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float WindRipplesIntensity = 0.25f;

	/** Speed of the Wind Ripples. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Ripples", DisplayName = "Wind Ripples Speed", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "4.0"))
	float WindRipplesSpeed = 1.0f;


	/** Snow Coverage Amount. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Snow", DisplayName = "Snow Amount", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowAmount = 0.0f;

	/** Snow Coverage Color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Snow", DisplayName = "Snow Color", meta = (HideAlphaChannel))
	FLinearColor SnowColor = FLinearColor(0.9, 0.9, 0.9, 1);

	/** Snow normap map intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Snow", DisplayName = "Snow Intensity", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowIntensity = 0.5f;

	/** Snow Sparkles Threshold. Lower values exposes more sparkles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX|Snow", DisplayName = "Snow Sparkles Threshold", meta = (ClampMin = "0.0", UIMin = "0.8", UIMax = "1.0"))
	float SnowSparklesThreshold = 0.95f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorWindSettings
{
	GENERATED_BODY()

	/** Wind direction angle of Low Cloud layer in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Wind Direction", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float CloudWindDirection = 0.0f;

	/** Wind speed of Low Cloud layer in m/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Wind Speed", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
	float CloudWindSpeed = 0.0f;

	/** Wind direction angle of High Cloud layer in degrees. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "High Cloud Wind Direction", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
//	float HighCloudWindDirection = 0.0f;

	/** Wind speed of High Cloud layer in m/s. */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "High Cloud Wind Speed", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
//	float HighCloudWindSpeed = 0.0f;

	/** Wind direction angle of Cloud Noise in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Noise Shape Wind Direction", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float CloudNoiseShapeWindDirection = 0.0f;

	/** Horizontal wind speed of Cloud Noise Shape in m/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Noise Shape Wind Speed Horizontal", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
	float CloudNoiseShapeWindSpeedHorizontal = 0.0f;

	/** Vertical wind speed of Cloud Noise Detail in m/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Noise Detail Wind Speed Vertical", meta = (UIMin = "-100.0", UIMax = "100.0"))
	float CloudNoiseDetailWindSpeedVertical = 2.0f;

	/** Wind direction angle of precipitation particles in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Weather FX", DisplayName = "Precipitation Wind Direction", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float PrecipitationWindDirection = 0.0f;

	/** Wind speed of precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Weather FX", DisplayName = "Precipitation Wind Speed", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "100.0"))
	float PrecipitationWindSpeed = 0.0f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorPostProcessSettings
{
	GENERATED_BODY()

	/** Multiplier for all bloom contributions >=0: off, 1(default), >1 brighter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Post Process", DisplayName = "Bloom Intensity", meta = (ClampMin = "0.0", UIMax = "8.0"))
	float BloomIntensity = 0.675f;

	/**
	* minimum brightness the bloom starts having effect
	* -1:all pixels affect bloom equally (physically correct, faster as a threshold pass is omitted), 0:all pixels affect bloom brights more, 1(default), >1 brighter
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Post Process", DisplayName = "Bloom Threshold", meta = (ClampMin = "-1.0", UIMax = "8.0"))
	float BloomThreshold = -1.0f;

	/**
	* Logarithmic adjustment for the exposure. Only used if a tonemapper is specified.
	* 0: no adjustment, -1:2x darker, -2:4x darker, 1:2x brighter, 2:4x brighter, ...
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Post Process", DisplayName = "Exposure Compensation", meta = (ClampMin = "-10.0", ClampMax = "10.0", UIMin = "-10.0", UIMax = "10.0"))
	float ExposureCompensation = 1.0f;
};

USTRUCT(BlueprintType)
struct FSkyCreatorWeatherSettings
{
	GENERATED_BODY()

	/** Sky Atmosphere Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Atmosphere", DisplayName = "Atmosphere Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSkyAtmosphereSettings SkyAtmosphereSettings;

	/** Volumetric Cloud Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Volumetric Clouds", DisplayName = "Volumetric Clouds Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorVolumetricCloudSettings VolumetricCloudSettings;

	/** Volumetric Cloud Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Background Clouds", DisplayName = "Background Clouds Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorBackgroundCloudSettings BackgroundCloudSettings;

	/** Sky Light Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sky Light", DisplayName = "Sky Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSkyLightSettings SkyLightSettings;

	/** Sun Light Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Sun Light", DisplayName = "Sun Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorSunLightSettings SunLightSettings;

	/** Moon Light Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Moon Light Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorMoonLightSettings MoonLightSettings;

	/** Exponential Height Fog Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Height Fog", DisplayName = "Height Fog Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorExponentialHeightFogSettings ExponentialHeightFogSettings;

	/** Star Map Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Star Map", DisplayName = "Star Map Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorStarMapSettings StarMapSettings;

	/** Weather FX Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Weather FX", DisplayName = "Weather FX Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWeatherFXSettings WeatherFXSettings;

	/** Weather Material FX Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Material FX", DisplayName = "Material FX Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWeatherMaterialFXSettings WeatherMaterialFXSettings;

	/** Wind Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind", DisplayName = "Wind Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorWindSettings WindSettings;

	/** Post Process Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Post Process", DisplayName = "Post Process Settings", meta = (ShowOnlyInnerProperties))
	FSkyCreatorPostProcessSettings PostProcessSettings;

};
