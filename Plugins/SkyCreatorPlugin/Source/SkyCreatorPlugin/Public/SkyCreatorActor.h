// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tickable.h"

#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/PostProcessComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SkyCreatorWeatherSettings.h"
#include "SkyCreatorWeatherPreset.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollectionInstance.h"
#include "TimerManager.h"
#include "SkyCreatorFunctionLibrary.h"
#include "SkyCreatorActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightningStrike, FVector, LightningPosition);

UENUM(BlueprintType)
enum ESkyCreatorEditorWeatherType
{
	EditorWeather_WeatherPreset UMETA(DisplayName = "Weather Preset"),
	EditorWeather_WeatherSettings UMETA(DisplayName = "Weather Settings")
};

UENUM(BlueprintType)
enum ESkyCreatorSunPositionType
{
	SunPositionType_Simple UMETA(DisplayName = "Simple"),
	SunPositionType_Real UMETA(DisplayName = "Real")
};

UENUM(BlueprintType)
enum ESkyCreatorMoonPositionType
{
	MoonPositionType_Simple UMETA(DisplayName = "Simple"),
	MoonPositionType_SimpleFixedElevation UMETA(DisplayName = "Simple w/ Fixed Elevation"),
	MoonPositionType_Real UMETA(DisplayName = "Real")
};

UENUM(BlueprintType)
enum EVolumetricCloudRenderTargetMode
{
	VolumetricCloudRenderTargetMode_Default UMETA(DisplayName = "Default"),
	VolumetricCloudRenderTargetMode_Quality UMETA(DisplayName = "Quality"),
	VolumetricCloudRenderTargetMode_Performance UMETA(DisplayName = "Performance")
};

UENUM(BlueprintType)
enum EVolumetricCloudNoiseShapeResolution
{
	VolumetricCloudNoiseShapeResolution_64 UMETA(DisplayName = "64 x 64 x 64"),
	VolumetricCloudNoiseShapeResolution_128 UMETA(DisplayName = "128 x 128 x 128"),
	VolumetricCloudNoiseShapeResolution_256 UMETA(DisplayName = "256 x 256 x 256")
};

UENUM(BlueprintType)
enum EVolumetricCloudNoiseDetailResolution
{
	VolumetricCloudNoiseDetailResolution_32 UMETA(DisplayName = "32 x 32 x 32"),
	VolumetricCloudNoiseDetailResolution_64 UMETA(DisplayName = "64 x 64 x 64")
};

UENUM(BlueprintType)
enum ESkyCreatorStarMapRotationType
{
	StarMapRotationType_NoRotation UMETA(DisplayName = "No Rotation"),
//	StarMapRotationType_FixedRotation UMETA(DisplayName = "Fixed Rotation"),
	StarMapRotationType_FollowSun UMETA(DisplayName = "Follow Sun"),
	StarMapRotationType_FollowMoon UMETA(DisplayName = "Follow Moon")
};

USTRUCT(BlueprintType)
struct FSkyCreatorLightningParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather FX", DisplayName = "Position")
	FLinearColor Position = FLinearColor(0.0f, 0.0f, 0.0f, 1000.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather FX", DisplayName = "Color")
	FLinearColor Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
};

UCLASS(BlueprintType)
class SKYCREATORPLUGIN_API ASkyCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyCreator();

	/** Allows Tick To happen in the editor viewport*/
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void PostActorCreated() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void PostEditChange();

	UFUNCTION(BlueprintImplementableEvent, CallInEditor, BlueprintCallable, Category = "Add Event")
	void EditorTick(float DeltaTime);
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ASkyCreator(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UBillboardComponent* Billboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UStaticMeshComponent* Compass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UVolumetricCloudComponent* VolumetricCloud;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	USkyLightComponent* SkyLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UDirectionalLightComponent* SunLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UDirectionalLightComponent* MoonLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UExponentialHeightFogComponent* ExponentialHeightFog;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UPostProcessComponent* PostProcess;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UStaticMeshComponent* StarMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UStaticMeshComponent* SunSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UStaticMeshComponent* MoonSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	USceneCaptureComponent2D* OcclusionCapture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UNiagaraComponent* WeatherFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator|Components")
	UStaticMeshComponent* SkySphere;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator")
	UMaterialInterface* SkySphereMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sky Creator")
	UMaterialInstanceDynamic* SkySphereMID;

	/**
	* Controls Time of Day by Editor.
	* Disable if using a separate controller to drive Time of Day.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", DisplayName = "Use Editor Time of Day")
	bool bUseEditorTimeOfDay = true;

	/** Time of Day in Editor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "General", DisplayName = "Editor Time of Day", meta = (UIMin = "0.0", UIMax = "24.0", EditCondition = "bUseEditorTimeOfDay"))
	float EditorTimeOfDay = 12.0f;

	/**
	* Controls Weather Settings by Editor.
	* Disable if using a separate controller to drive Weather Settings.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", DisplayName = "Use Editor Weather Settings")
	bool bUseEditorWeatherSettings = true;

	/** Whether to use Editor Weather Preset or Editor Weather Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", DisplayName = "Editor Weather Type", meta = (EditCondition = "bUseEditorWeatherSettings"))
	TEnumAsByte<ESkyCreatorEditorWeatherType> EditorWeatherType = ESkyCreatorEditorWeatherType::EditorWeather_WeatherPreset;

	/** Current Weather Preset in Editor. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "General", DisplayName = "Editor Weather Preset", meta = (EditCondition = "EditorWeatherType == ESkyCreatorEditorWeatherType::EditorWeather_WeatherPreset && bUseEditorWeatherSettings"))
	USkyCreatorWeatherPreset* EditorWeatherPreset;

	/** Current Weather Settings in Editor allowing to direct variable control. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General", DisplayName = "Editor Weather Settings", meta = (EditCondition = "EditorWeatherType == ESkyCreatorEditorWeatherType::EditorWeather_WeatherSettings && bUseEditorWeatherSettings"))
	FSkyCreatorWeatherSettings EditorWeatherSettings;
	
	/**
	 * Marks if the actor is used by Sequencer in runtime.
	 * This is a legacy feature specific only for UE4.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "General", DisplayName = "Is Used With Sequencer")
	bool bIsUsedWithSequencer = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "General", DisplayName = "Is Editor Tick Enabled")
	bool bIsEditorTickEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateTime, Category = "General", DisplayName = "Time of Day", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float TimeOfDay = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateWeather, Category = "General", DisplayName = "Weather Settings", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FSkyCreatorWeatherSettings WeatherSettings;

	/** Common Material Parameter Collection. Essential for most of effects and settings related to materials. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "General", DisplayName = "Common Parameter Collection")
	UMaterialParameterCollection* CommonMPC;

	/** Sky Sphere mesh radius in kilometers. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "General", DisplayName = "Sky Sphere Radius", meta = (ClampMin = "100.0", ClampMax = "10000.0", UIMin = "100.0"))
	float SkySphereRadius = 1000.0f;

	/** Show or Hide debug variables across all sub-categories of settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "General", DisplayName = "Show Debug Variables")
	bool bShowDebugVariables = false;



	/** Sun Position Type. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position", DisplayName = "Sun Position Type")
	TEnumAsByte<ESkyCreatorSunPositionType> SunPositionType = ESkyCreatorSunPositionType::SunPositionType_Simple;

	/** Sun Position Data. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position", DisplayName = "Sun Position Data")
	FCelestialPositionData SunPositionData;

	/** Moon Position Type. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position", DisplayName = "Moon Position Type")
	TEnumAsByte<ESkyCreatorMoonPositionType> MoonPositionType = ESkyCreatorMoonPositionType::MoonPositionType_Simple;

	/** Moon Position Data. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position", DisplayName = "Moon Position Data")
	FCelestialPositionData MoonPositionData;


	/** User-defined Sunrise Time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sunrise Time", meta = (ClampMin = "0.0", ClampMax = "24.0", UIMin = "0.0", UIMax = "24.0", EditCondition = "SunPositionType == ESkyCreatorSunPositionType::SunPositionType_Simple"))
	float SunriseTime = 6.5f;

	/** User-defined Sunset Time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sunset Time", meta = (ClampMin = "0.0", ClampMax = "24.0", UIMin = "0.0", UIMax = "24.0", EditCondition = "SunPositionType == ESkyCreatorSunPositionType::SunPositionType_Simple"))
	float SunsetTime = 19.5f;

	/** Sets "Sun Dawn Time" depending on "Sunrise Time" */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Dawn Offset Time", meta = (ClampMin = "0.0", ClampMax = "4.0", UIMin = "0.0", UIMax = "2.0"))
	float SunDawnOffsetTime = 1.5f;

	/** Sets "Sun Dusk Time" depending on "Sunset Time" */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Dusk Offset Time", meta = (ClampMin = "0.0", ClampMax = "4.0", UIMin = "0.0", UIMax = "2.0"))
	float SunDuskOffsetTime = 1.5f;

	/** Sun Dawn Time. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Dawn Time")
	float SunDawnTime = 0.0f;

	/** Sun Dusk Time. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Dusk Time")
	float SunDuskTime = 0.0f;

	/** Sun Elevation angle in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Elevation", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"))
	float SunElevation = 55.0f;

	/** Sun Azimuth angle in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Azimuth", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float SunAzimuth = 270.0f;

	/** Minimum Sun angle below horizon at Dawn or Dusk */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Min Angle At Dawn & Dusk", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
	float SunMinAngleAtDawnDusk = 8.0f;

	/** Fade in/out time for Sun at Dawn or Dusk */
	UPROPERTY(BlueprintReadOnly, Category = "Sun & Moon Position|Simple Position", DisplayName = "Sun Fade In/Out Time", meta = (ClampMin = "0.05", ClampMax = "2.0", UIMin = "0.05", UIMax = "0.5"))
	float SunFadeInOutTime = 0.1f;


	/** User-defined Moonrise Time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Moonrise Time", meta = (ClampMin = "0.0", ClampMax = "24.0", UIMin = "0.0", UIMax = "24.0", EditCondition = "MoonPositionType == ESkyCreatorMoonPositionType::MoonPositionType_Simple"))
	float MoonriseTime = 18.0f;

	/** User-defined Moonset Time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Moonset Time", meta = (ClampMin = "0.0", ClampMax = "24.0", UIMin = "0.0", UIMax = "24.0", EditCondition = "MoonPositionType == ESkyCreatorMoonPositionType::MoonPositionType_Simple"))
	float MoonsetTime = 8.0f;

	/** Moon Elevation angle in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Moon Elevation", meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"))
	float MoonElevation = 45.0f;

	/** Moon Azimuth angle in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Simple Position", DisplayName = "Moon Azimuth", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float MoonAzimuth = 270.0f;

	/** Fade in/out time for Moon at Dawn or Dusk */
	UPROPERTY(BlueprintReadOnly, Category = "Sun & Moon Position|Simple Position", DisplayName = "Moon Fade In/Out Time", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float MoonFadeInOutTime = 0.1f;


	/** Latitude for calculating Real Sun & Moon positioning. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Latitude", meta = (UIMin = "-90.0", UIMax = "90.0", ClampMin = "-90.0", ClampMax = "90.0"))
	float Latitude = 51.509865f;

	/** Longitude for calculating Real Sun & Moon positioning. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Longitude", meta = (UIMin = "-180.0", UIMax = "180.0", ClampMin = "-180.0", ClampMax = "180.0"))
	float Longitude = -0.118092f;

	/** Time Zone. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Time Zone", meta = (UIMin = "-12.0", UIMax = "12.0", ClampMin = "-12.0", ClampMax = "12.0"))
	float TimeZone = 0.0f;

	/** Enables Daylight Saving Time (DST). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Daylight Saving Time")
	bool bDaylightSavingTime = false;

	/** Year. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Year", meta = (UIMin = "1900.0", UIMax = "2100.0")) //, ClampMin = "1990.0", ClampMax = "2100.0", ))
	int32 Year = 2022;

	/** Month. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Month", meta = (UIMin = "1.0", UIMax = "12.0", ClampMin = "1.0", ClampMax = "12.0"))
	int32 Month = 5;

	/** Day. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun & Moon Position|Real Position", DisplayName = "Day", meta = (UIMin = "1.0", UIMax = "31.0", ClampMin = "1.0", ClampMax = "31.0"))
	int32 Day = 10;

	/** Calculated Date. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Real Position", DisplayName = "Date", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FDateTime Date = NULL;


	/**
	* Enables Light Transition feature which optimizes performance and smoothly transitions between Sun Light & Moon Light.
	* Assumes that Sun Light is always the dominant light source and that both light sources are always affecting atmosphere.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Light Transition")
	bool bLightTransition = true;

	/** Current Sun Surface Brightness. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Sun Surface Brightness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "bShowDebugVariables", EditConditionHides))
	float SunSurfaceBrightness = 1.0f;

	/** Current Moon Surface Brightness. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Moon Surface Brightness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "bShowDebugVariables", EditConditionHides))
	float MoonSurfaceBrightness = 1.0f;

	/** Current Sun Elevation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Sun Current Elevation", meta = (ClampMin = "-90.0", ClampMax = "90.0", UIMin = "-90.0", UIMax = "90.0"))
	float SunCurrentElevation = 0.0f;

	/**
	* The angle of the Sun to start the light transition.
	* From this angle to "Transition Middle Sun Angle" the surface lighting of the Sun linearly fades out.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Transition Start Sun Angle", meta = (UIMin = "-18.0", UIMax = "18.0"))
	float TransitionStartSunAngle = -6.0f;

	/**
	* The angle to switch the transition.
	* At this angle the Shadow casting feature is switching between Sun & Moon as they're should not cast shadows simultaneously.
	* From this angle to "Transition End Sun Angle" the surface lighting of the Moon linearly fades in.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Transition Middle Sun Angle", meta = (UIMin = "-18.0", UIMax = "18.0"))
	float TransitionMiddleSunAngle = 0.0f;

	/**
	* The angle to end the transition.
	* From this angle to "Transition Middle Sun Angle" the surface lighting of the Moon linearly fades in.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Transition End Sun Angle", meta = (UIMin = "-18.0", UIMax = "18.0"))
	float TransitionEndSunAngle = 6.0f;

	/**
	* The angle of the Sun to start the transition to Sky Light Night Intensity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Night Intensity Transition Start Sun Angle", meta = (UIMin = "-18.0", UIMax = "18.0"))
	float NightIntensityTransitionStartSunAngle = 3.0f;

	/**
	* The angle of the Sun to end the transition to Sky Light Night Intensity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun & Moon Position|Light Transition", DisplayName = "Night Intensity Transition End Sun Angle", meta = (UIMin = "-18.0", UIMax = "18.0"))
	float NightIntensityTransitionEndSunAngle = 9.0f;



	/** Sky Atmosphere Mobility. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere", DisplayName = "Sky Atmosphere Mobility")
	TEnumAsByte<EComponentMobility::Type> SkyAtmosphereMobility = EComponentMobility::Type::Movable;

	/**
	* - Planet Top at Absolute World Position places the top ground level of the atmosphere at the world origin 
	* coordinates (0,0,0) in the scene. The Sky Atmosphere is not movable when this option is selected.
	* - Planet Top at Component Transform places the top ground level of the atmosphere relative to the component's 
	* transform origin. Moving the transform of the Sky Atmosphere component, or one that it is a child of, 
	* moves the atmosphere within the level
	* - Planet Center at Component Transform places the atmosphere centered to the component's transform origin. 
	* Moving the transform of the Sky Atmosphere component, or one that it is a child of, moves the atmosphere within the level.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere", DisplayName = "Transform Mode")
	ESkyAtmosphereTransformMode TransformMode = ESkyAtmosphereTransformMode::PlanetTopAtComponentTransform;

	/** The planet radius (kilometers from the center to the ground level). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere", DisplayName = "Planet Radius", meta = (ClampMin = "0.1", ClampMax = "10000.0", UIMin = "1.0", UIMax = "7000.0", SliderExponent = "5.0"))
	float PlanetRadius = 6360.0f;

	/** Atmosphere Height defines the height of the atmosphere above the planet's surface (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere", DisplayName = "Atmosphere Height", meta = (ClampMin = "0.1", ClampMax = "200.0", UIMin = "1.0", UIMax = "200.0", SliderExponent = "5.0"))
	float AtmosphereHeight = 60.0f;

	/** Makes the aerial perspective look thicker by scaling distances from view to surfaces (opaque and translucent). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere", meta = (UIMin = 0.0, UIMax = 3.0, ClampMin = 0.0, SliderExponent = 2.0))
	float AerialPespectiveViewDistanceScale = 1.0f;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Atmosphere", DisplayName = "Planet Top Position", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector PlanetTopPosition = FVector::ZeroVector;

	/**
	* Scale the atmosphere tracing sample count. Quality level scalability
	* The sample count is still clamped according to scalability setting to 'r.SkyAtmosphere.SampleCountMax' when 'r.SkyAtmosphere.FastSkyLUT' is 0.
	* The sample count is still clamped according to scalability setting to 'r.SkyAtmosphere.FastSkyLUT.SampleCountMax' when 'r.SkyAtmosphere.FastSkyLUT' is 1.
	* The sample count is still clamped for aerial perspective according to  'r.SkyAtmosphere.AerialPerspectiveLUT.SampleCountMaxPerSlice'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere|Quality", DisplayName = "Trace Sample Count Scale", meta = (ClampMin = "0.25", UIMin = "0.25", UIMax = "8.0"))
	float TraceSampleCountScale = 4.0f;

	/** This value controls Aerial Perspective LUT Width. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Atmosphere|Quality", DisplayName = "God Rays Resolution", meta = (ClampMin = "0.0", UIMin = "16.0", UIMax = "128.0"))
	float GodRaysResolution = 32.0f;


	/** The altitude at which the cloud layer starts (kilometers above the ground). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Volumetric Clouds", DisplayName = "Bottom Altitude", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "20.0"))
	float LayerBottomAltitude = 2.0f;

	/** The altitude at which the cloud layer ends (kilometers above the ground). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Volumetric Clouds", DisplayName = "Max Height", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "20.0"))
	float LayerHeight = 8.0f;

	/** The maximum distance of the volumetric surface before which we will accept to start tracing (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Tracing Start Max Distance", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "500.0"))
	float TracingStartMaxDistance = 350.0f;

	/** The maximum distance that will be traced inside the cloud layer (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Tracing Max Distance", meta = (ClampMin = "0.1", UIMin = "1.0", UIMax = "500.0"))
	float TracingMaxDistance = 50.0f;

	/** Whether to apply atmosphere transmittance per sample, instead of using the light global transmittance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Per Sample Atmospheric Light Transmittance")
	bool bPerSampleAtmosphericLightTransmittance = true;

	/** Essential Material Parameter Collection for Volumetric Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "Volumetric Clouds", DisplayName = "Volumetric Cloud Parameter Collection")
	UMaterialParameterCollection* VolumetricCloudsMPC;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Bottom Altitude Position", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector LayerBottomAltitudePosition = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Top Altitude Position", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector LayerTopAltitudePosition = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Current Volumetric Cloud Material", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInterface* CurrentVolumetricCloudMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Volumetric Clouds", DisplayName = "Volumetric Cloud MID", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInstanceDynamic* VolumetricCloudMID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Volumetric Cloud Density Sample Material", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInterface* VolumetricCloudDensitySampleMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Volumetric Cloud Density Sample MID", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInstanceDynamic* VolumetricCloudDensitySampleMID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds", DisplayName = "Volumetric Cloud Density Sample Render Target", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* VolumetricCloudDensitySampleRT;


	/**
	* Enables cinematic quality on Volumetric Cloud Render Target.
	* Disables any form of Volumetric Cloud Render Target optimizations.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Cinematic Quality")
	bool bCinematicQuality = false;

	/**
	* Default: trace quarter resolution + reconstruct at half resolution + upsample
	* Quality: trace half resolution + reconstruct full res + upsample
	* Performance: trace at quarter resolution + reconstruct full resolution (cannot intersect with opaque meshes and forces UpsamplingMode = 2)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Render Mode", meta = (EditCondition = "!bCinematicQuality"))
	TEnumAsByte<EVolumetricCloudRenderTargetMode> RenderMode = EVolumetricCloudRenderTargetMode::VolumetricCloudRenderTargetMode_Default;

	/**
	* Enable/disable a second pass to trace the aerial perspective per pixel on clouds instead of using the aerial perspective texture.
	* Only usable when r.VolumetricCloud.EnableAerialPerspectiveSampling = 1 and only needed for extra quality when r.VolumetricRenderTarget = 1.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "High Quality Aerial Perspective")
	bool bHighQualityAerialPerspective = false;

	/**
	* How many octave to use for the multiple-scattering approximation. 
	* This makes the shader more expensive so try to use only a single octave.
	* 0 means single scattering only.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Multi Scattering Approximation Octave Count", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	int32 MultiScatteringApproximationOctaveCount = 1;

	/**
	* Sample the shadowed lighting contribution from the ground onto the medium (single scattering).
	* This adds some costs to the tracing when enabled.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Ground Contribution")
	bool bGroundContribution = false;

	/**
	* Disable this to use the cloud shadow map instead of secondary raymarching.
	* This is usually enough for clouds viewed from the ground and it result in a performance boost.
	* Shadow now have infinite length but also becomes less accurate and gray scale.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Ray March Volume Shadow")
	bool bRayMarchVolumeShadow = true;

	/**
	* Scale the tracing sample count in primary views. Quality level scalability CVARs affect the maximum range.
	* The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ViewRaySampleCountMax'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "View Sample Count Scale", meta = (ClampMin = "0.25", UIMin = "0.25", UIMax = "8.0"))
	float ViewSampleCountScale = 1.5f;

	/**
	 * Scale the tracing sample count in reflection views. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ReflectionRaySampleMaxCount'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Reflection Sample Count Scale", meta = (ClampMin = "0.25", UIMin = "0.25", UIMax = "8.0"))
	float ReflectionSampleCountScale = 0.5f;

	/**
	 * Scale the shadow tracing sample count in primary views, only used with Advanced Output ray marched shadows. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.Shadow.ViewRaySampleMaxCount'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Shadow View Sample Count Scale", meta = (EditCondition = "bRayMarchVolumeShadow", ClampMin = "0.25", UIMin = "0.25", UIMax = "8.0"))
	float ShadowViewSampleCountScale = 0.5f;

	/**
	* Scale the shadow tracing sample count in reflection views, only used with Advanced Output ray marched shadows. Quality level scalability CVARs affect the maximum range.
	* The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.Shadow.ReflectionRaySampleMaxCount'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Shadow Reflection Sample Count Scale", meta = (ClampMin = "0.25", UIMin = "0.25", UIMax = "8.0"))
	float ShadowReflectionSampleCountScale = 0.25f;

	/** The shadow tracing distance in kilometers, only used with Advanced Output ray marched shadows. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Quality", DisplayName = "Shadow Tracing Distance", meta = (EditCondition = "bRayMarchVolumeShadow", ClampMin = "0.01", UIMin = "0.1", UIMax = "50.0"))
	float ShadowTracingDistance = 5.0f;

	/** Cloud Map tileable texture scale in kilometers. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Map", DisplayName = "Cloud Map Scale", meta = (ClampMin = "10.0", UIMin = "1.0", UIMax = "200.0"))
	float CloudMapScale = 50.0f;

	/** Low Cloud Map tileable texture UV offset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Volumetric Clouds|Cloud Map", DisplayName = "Cloud Map Offset")
	FVector2D CloudMapOffset = FVector2D(0, 0);

	/** Coverage Variation Map tileable texture scale in kilometers. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Map", DisplayName = "Coverage Variation Map Scale", meta = (ClampMin = "10.0", UIMin = "1.0", UIMax = "200.0"))
	float CoverageVariationMapScale = 50.0f;


	/**
	* Resolution of a Noise Shape 3D Texture. Can affect performance and memory consumption.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Noise", DisplayName = "Noise Shape Resolution")
	TEnumAsByte<EVolumetricCloudNoiseShapeResolution> NoiseShapeResolution = EVolumetricCloudNoiseShapeResolution::VolumetricCloudNoiseShapeResolution_64;

	/**
	* Resolution of a Noise Detail 3D Texture. Can affect performance and memory consumption.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Noise", DisplayName = "Noise Detail Resolution")
	TEnumAsByte<EVolumetricCloudNoiseDetailResolution> NoiseDetailResolution = EVolumetricCloudNoiseDetailResolution::VolumetricCloudNoiseDetailResolution_32;

	/** Tileable 3D Noise Shape Scale (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Noise", DisplayName = "Noise Shape Scale", meta = (ClampMin = "0.1", UIMin = "1.0", UIMax = "10.0"))
	float NoiseShapeScale = 6.0f;

	/** Tileable 3D Noise Detail Scale (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Cloud Noise", DisplayName = "Noise Detail Scale", meta = (ClampMin = "0.1", UIMin = "0.25", UIMax = "4.0"))
	float NoiseDetailScale = 0.75f;


	/** Tileable Turbulence Scale (in kilometers). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Volumetric Clouds|Turbulence", DisplayName = "Turbulence Scale", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "16.0"))
	float TurbulenceScale = 4.0f;



	/** Background Clouds Contrast. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Background Clouds", DisplayName = "Background Clouds Contrast", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "6.0"))
	float BackgroundCloudsContrast = 3.0f;

	/** Affects the scale of reflection injected to sky light. When set to 0, Background Clouds are not illuminating the world. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Background Clouds", DisplayName = "Background Clouds Reflection Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BackgroundCloudsReflectionScale = 0.5f;



	/** Sky Light Mobility. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light", DisplayName = "Sky Light Mobility")
	TEnumAsByte<EComponentMobility::Type> SkyLightMobility = EComponentMobility::Type::Movable;

	/**
	* When enabled, the sky will be captured and convolved to achieve dynamic diffuse and specular environment lighting.
	* SkyAtmosphere, VolumetricCloud Components as well as sky domes with Sky materials are taken into account.
	* Should be enabled if using dynamic time of day.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light", DisplayName = "Real Time Capture")
	bool bRealTimeCapture = true;

	/** When enabled, the real-time sky light capture and convolutions will by distributed over several frames to lower the per-frame cost. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light", DisplayName = "Capture Time Slice")
	bool bSkyLightCaptureTimeSlice = true;

	/**
	 * Whether all distant lighting from the lower hemisphere should be set to LowerHemisphereColor.
	 * Enabling this is accurate when lighting a scene on a planet where the ground blocks the sky,
	 * However disabling it can be useful to approximate skylight bounce lighting (eg Movable light).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light", DisplayName = "Lower Hemisphere Is Solid Color")
	bool bLowerHemisphereIsSolidColor = true;

	/** Whether the cloud should occlude sky contribution within the atmosphere (progressively fading multiple scattering out) or not. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light|Atmosphere And Cloud")
	bool bCloudAmbientOcclusion = true;

	/** The world space radius of the cloud ambient occlusion map around the camera in kilometers. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light|Atmosphere And Cloud", meta = (UIMin = "1", ClampMin = "1"))
	float CloudAmbientOcclusionExtent = 50.0f;

	/** Scale the cloud ambient occlusion map resolution, base resolution is 512. The resolution is still clamped to 'r.VolumetricCloud.SkyAO.MaxResolution'. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light|Atmosphere And Cloud", meta = (UIMin = "0.25", UIMax = "8", ClampMin = "0.25", SliderExponent = 1.0))
	float CloudAmbientOcclusionMapResolutionScale = 1.0f;

	/**
	* Controls the cone aperture angle over which the sky occlusion due to volumetric clouds is evaluated. 
	* A value of 1 means `take into account the entire hemisphere` resulting in blurry occlusion, 
	* while a value of 0 means `take into account a single up occlusion direction up` resulting in sharp occlusion.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky Light|Atmosphere And Cloud", meta = (UIMin = "0.0", UIMax = "0.1", ClampMin = "0.0", ClampMax = "1.0", SliderExponent = 2.0))
	float CloudAmbientOcclusionApertureScale = 0.05f;



	/** Sun Light Mobility. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Sun Light Mobility")
	TEnumAsByte<EComponentMobility::Type> SunLightMobility = EComponentMobility::Type::Movable;

	/** Whether to control Sun direction automatically by Sky Creator. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Control Sun Direction")
	bool bControlSunDirection = true;

	/** Whether to control Sun Intensity by a constant value or by a value from a Weather Preset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Sun Constant Intensity")
	bool bSunConstantIntensity = true;

	/** Sun Constant Intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun Light", DisplayName = "Sun Intensity", meta = (EditCondition = "bSunConstantIntensity", ClampMin = "0.0", UIMin = "0.0"))
	float SunIntensity = 10.0f;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Sun Current Intensity", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float SunCurrentIntensity = 0.0f;

	/** Whether to use Color temperature for sun. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light", DisplayName = "Use Temperature")
	bool bSunUseTemperature = false;

	/**
	 * Angle subtended by light source in degrees (also known as angular diameter).
	 * Realistic value is 0.5357 which is the angle for our sun.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun Light", DisplayName = "Sun Disk Size", meta = (ClampMin = "0.0", ClampMax = "60.0", UIMin = "0.0", UIMax = "20.0"))
	float SunDiskSize = 2.0f;

	/** Whether to use constant Atmosphere Disk Color Scale for Sun. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Constant Atmosphere Disk Color Scale")
	bool bSunConstantAtmosphereDiskColorScale = false;

	/** A color multiplied with the sun disk luminance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Atmosphere Disk Color Scale", meta = (EditCondition = "bSunConstantAtmosphereDiskColorScale"))
	FLinearColor SunAtmosphereDiskColorScale = FLinearColor(1, 1, 1, 1);

	/** Whether to apply atmosphere transmittance per pixel on opaque meshes, instead of using the light global transmittance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Per Pixel Atmosphere Transmittance")
	bool bSunPerPixelAtmosphereTransmittance = false;

	/**
	* The world space radius of the cloud shadow map around the camera in kilometers.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Extent", meta = (UIMin = "1", ClampMin = "1"))
	float SunCloudShadowExtent = 35.0f;

	/**
	* Scale the cloud shadow map resolution, base resolution is 512. The resolution is still clamped to 'r.VolumetricCloud.ShadowMap.MaxResolution'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Map Resolution Scale", meta = (UIMin = "0.25", UIMax = "8", ClampMin = "0.25", SliderExponent = 1.0))
	float SunCloudShadowMapResolutionScale = 1.0f;

	/**
	* Scale the shadow map tracing sample count.
	* The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ShadowMap.RaySampleMaxCount'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sun Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Ray Sample Count Scale", meta = (UIMin = "0.25", UIMax = "8", ClampMin = "0.25", SliderExponent = 1.0))
	float SunCloudShadowRaySampleCountScale = 1.0f;



	/** Moon Light Mobility. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Moon Light Mobility")
	TEnumAsByte<EComponentMobility::Type> MoonLightMobility = EComponentMobility::Type::Movable;

	/** Whether to control Moon direction automatically by Sky Creator. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Control Moon Direction")
	bool bControlMoonDirection = true;

	/** Whether to control Moon Intensity by a constant value or by a value from a Weather Preset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Moon Constant Intensity")
	bool bMoonConstantIntensity = true;

	/** Moon Constant Intensity. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Moon Light", DisplayName = "Moon Intensity", meta = (EditCondition = "bMoonConstantIntensity", ClampMin = "0.0", UIMin = "0.0"))
	float MoonIntensity = 0.04f;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Moon Current Intensity", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float MoonCurrentIntensity = 0.0f;

	/** Whether to use Color temperature for moon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light", DisplayName = "Use Temperature")
	bool bMoonUseTemperature = false;

	/**
	* Angle subtended by light source in degrees (also known as angular diameter).
	* Realistic value is 0.5357 which is the angle for our sun.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Moon Disk Size", meta = (ClampMin = "0.0", ClampMax = "60.0", UIMin = "0.0", UIMax = "20.0"))
	float MoonDiskSize = 2.0f;

	/** Moon Disk Rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light", DisplayName = "Moon Rotation", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MoonRotation = 0.0f;

	/** Moon Disk Phase. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Moon Light|Moon Phase", DisplayName = "Moon Phase", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MoonPhase = 0.0f;

	/** Whether to scale Moon Light Intensity of the Moon Phase. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Moon Phase", DisplayName = "Moon Phase Light Intensity Scale")
	bool bMoonPhaseLightIntensityScale = false;

	/** Minimum Moon Light Intensity scale of the Moon Phase. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Moon Phase", DisplayName = "Moon Phase Light Intensity Min Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "bMoonPhaseLightIntensityScale"))
	float MoonPhaseLightIntensityMinScale = 0.5f;

	/** Maximum Moon Light Intensity scale of the Moon Phase. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Moon Phase", DisplayName = "Moon Phase Light Intensity Max Scale", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "bMoonPhaseLightIntensityScale"))
	float MoonPhaseLightIntensityMaxScale = 1.0f;


	/** Whether to use constant Atmosphere Disk Color Scale for Moon. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Constant Atmosphere Disk Color Scale")
	bool bMoonConstantAtmosphereDiskColorScale = false;

	/** A color multiplied with the moon disk luminance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Atmosphere Disk Color Scale", meta = (EditCondition = "bMoonConstantAtmosphereDiskColorScale"))
	FLinearColor MoonAtmosphereDiskColorScale = FLinearColor(1.0f, 0.88f, 0.4f);

	/** Whether to apply atmosphere transmittance per pixel on opaque meshes, instead of using the light global transmittance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Per Pixel Atmosphere Transmittance")
	bool bMoonPerPixelAtmosphereTransmittance = false;

	/**
	* The world space radius of the cloud shadow map around the camera in kilometers.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Extent", meta = (UIMin = "1", ClampMin = "1"))
	float MoonCloudShadowExtent = 35.0f;

	/**
	* Scale the cloud shadow map resolution, base resolution is 512. The resolution is still clamped to 'r.VolumetricCloud.ShadowMap.MaxResolution'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Map Resolution Scale", meta = (UIMin = "0.25", UIMax = "8", ClampMin = "0.25", SliderExponent = 1.0))
	float MoonCloudShadowMapResolutionScale = 1.0f;

	/**
	* Scale the shadow map tracing sample count.
	* The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ShadowMap.RaySampleMaxCount'.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moon Light|Atmosphere And Cloud", DisplayName = "Cloud Shadow Ray Sample Count Scale", meta = (UIMin = "0.25", UIMax = "8", ClampMin = "0.25", SliderExponent = 1.0))
	float MoonCloudShadowRaySampleCountScale = 1.0f;




	/** Exponential Height Fog Mobility. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Height Fog", DisplayName = "Height Fog Mobility")
	TEnumAsByte<EComponentMobility::Type> ExponentialHeightFogMobility = EComponentMobility::Type::Movable;

	/** Whether to enable Exponential Height Fog. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Height Fog", DisplayName = "Enable Height Fog")
	bool bEnableExponentialHeightFog = true;
	
	/** Whether to enable Volumetric fog. Scalability settings control the resolution of the fog simulation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Height Fog", DisplayName = "Volumetric Fog", meta = (EditCondition = "bEnableExponentialHeightFog"))
	bool bVolumetricFog = false;

	/** Fog height offset, relative to the actor position Z. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Height Fog", DisplayName = "Fog Height Offset", meta = (EditCondition = "bEnableExponentialHeightFog"))
	float FogHeightOffset = 0.0f;

	/** Second Fog height offset, relative to the actor position Z. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Height Fog", DisplayName = "Second Fog Height Offset", meta = (EditCondition = "bEnableExponentialHeightFog"))
	float SecondFogHeightOffset = 0.0f;



	/** Star Map Texture. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Star Map", DisplayName = "Star Map Texture")
	UTexture2D* StarMapTexture;

	/** Star Map Rotation Type. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Star Map", DisplayName = "Rotation Type")
	TEnumAsByte<ESkyCreatorStarMapRotationType> StarMapRotationType = ESkyCreatorStarMapRotationType::StarMapRotationType_FollowMoon;

	/** Star Map Additional Rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Interp, Category = "Star Map", DisplayName = "Additional Rotation")
	FVector StarMapAdditionalRotation = FVector(40, 120, 0);



	/** Whether to enable realtime Occlusion Capture. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture")
	bool bEnableOcclusionCapture = true;

	/** Occlusion Render Target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Render Target")
	UTextureRenderTarget2D* OcclusionRenderTarget;

	/** Occlusion Capture width. Half of this value is a radius. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture Width", meta = (ClampMin = "100.0", UIMin = "1000.0", UIMax = "20000.0"))
	float OcclusionCaptureWidth = 10000.0f;

	/** Occlusion Capture camera height. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture Height", meta = (ClampMin = "100.0", UIMin = "1000.0", UIMax = "50000.0"))
	float OcclusionCaptureHeight = 10000.0f;

	/** Occlusion Capture realtime update. Expensive, so use with caution. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture Realtime Update")
	bool bOcclusionCaptureRealtimeUpdate = false;

	/** Fixed distance step to update Occlusion Capture. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture Step Distance", meta = (EditCondition = "!bOcclusionCaptureRealtimeUpdate", ClampMin = "10.0", UIMin = "100.0", UIMax = "2000.0"))
	float OcclusionCaptureStepDistance = 500.0f;

	/**
	* Step Size of the Occlusion Capture. Also this will be the size of a single texel of the Occlusion Render Target in world units. 
	* This is done to avoid 'pixel bleeding'.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Capture Step Size", meta = (EditCondition = "bShowDebugVariables", EditConditionHides, ClampMin = "1.0", UIMin = "1.0", UIMax = "100.0"))
	float OcclusionCaptureStepSize = 10.0f;

	/** Occlusion Bias works similarly as known 'Shadow Bias' technique in shadow mapping. Low values can cause occlusion artifacts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Bias", meta = (ClampMin = "0.0", UIMin = "0.1", UIMax = "10.0"))
	float OcclusionBias = 5.0f;

	/** Sample count of the Occlusion Blur. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Blur Samples", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "256.0"))
	int32 OcclusionBlurSamples = 32;

	/** Blurring distance of the Occlusion Blur. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Blur Distance", meta = (ClampMin = "0.0", UIMin = "1.0", UIMax = "100.0"))
	float OcclusionBlurDistance = 20.0f;

	/** Hardness of fading Occlusion radial mask. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Occlusion Mask Fade Hardness", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float OcclusionMaskFadeHardness = 0.9f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Camera Location", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CameraLocation = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Occlusion", DisplayName = "Camera Location Snapped", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CameraLocationSnapped = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Occlusion", DisplayName = "Occlusion Current Location", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector OcclusionCurrentLocation = FVector(0, 0, 0);



	/** Cutoff World Height of the all WeatherFX. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX", DisplayName = "Cutoff World Height")
	float WeatherFXCutoffWorldHeight = -100.0f;

	/** Cutoff Softness of the all WeatherFX. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX", DisplayName = "Cutoff Softness")
	float WeatherFXCutoffSoftness = 100.0f;

	/** Precipitation Mode. */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Simulation Mode")
//	TEnumAsByte<ESkyCreatorPrecipitationSimulationMode> PrecipitationSimulationMode = ESkyCreatorPrecipitationSimulationMode::PrecipitationSim_CPUGPU;

	/** Precipitation spawn radius around the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Spawn Radius", meta = (UIMin = "200.0", UIMax = "1000.0"))
	float PrecipitationSpawnRadius = 600.0f;

	/**
	* Whether to enable GPU Precipitation feature. Requires Occlusion Capture to be enabled.
	* This is relatively cheap to use and improves overall precipitation particle density.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Enable GPU Precipitation", meta = (EditCondition = "bEnableOcclusionCapture"))
	bool bEnableGPUPrecipitation = true;

	/** GPU Precipitation spawn radius around the camera. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Spawn Radius GPU", meta = (EditCondition = "bEnableGPUPrecipitation && bEnableOcclusionCapture", UIMin = "1000.0", UIMax = "4000.0"))
	float PrecipitationSpawnRadiusGPU = 2000.0f;

	/** Max distance at where precipitation particles will live. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Max View Distance")
	float PrecipitationMaxViewDistance = 4000.0f;

	/** Distance of vertical line trace from particle spawn point to check against occlusion. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Vertical Check Distance")
	float PrecipitationVerticalCheckDistance = 10000.0f;

	/** Collision Channel for precipitation particles to check against. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Collision Channel")
	TEnumAsByte<ECollisionChannel> PrecipitationCollisionChannel = ECollisionChannel::ECC_WorldStatic;

	/** Depth Fade distance for precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Depth Fade Distance")
	float PrecipitationDepthFadeDistance = 100.0f;

	/** Camera Fade distance for precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Camera Fade Distance")
	float PrecipitationCameraFadeDistance = 100.0f;

	/** Camera Fade Offset distance for precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Precipitation General", DisplayName = "Precipitation Camera Fade Offset")
	float PrecipitationCameraFadeOffset = 50.0f;



	/**
	* Max number of rain particles to spawn per second. 
	* Value is multiplied by "Rain Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Spawn Rate Max (CPU)", meta = (UIMin = "100.0", UIMax = "10000.0"))
	float RainSpawnRateMaxCPU = 4000.0f;

	/**
	* Max number of rain particles to spawn per second.
	* Value is multiplied by "Rain Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Spawn Rate Max (GPU)", meta = (EditCondition = "bEnableGPUPrecipitation && bEnableOcclusionCapture", UIMin = "10000.0", UIMax = "200000.0"))
	float RainSpawnRateMaxGPU = 100000.0f;

	/** Scale Factor by distance for rain particles (for better visibility). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Distance Scale Factor")
	float RainDistanceScaleFactor = 2.0f;

	/**
	* Sets the amount of Camera Motion Alignment effect.
	* Set 0 to turn it off.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Camera Motion Alignment Scale", meta = (UIMin = "0.0", UIMax = "1.0"))
	float RainCameraMotionAlignmentScale = 0.6f;

	/**
	* Elongation (stretching) scale based on velocity of a rain particle.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Velocity Elongation Scale", meta = (UIMin = "0.0", UIMax = "2.0"))
	float RainVelocityElongationScale = 2.0f;

	/**
	* Index of Refraction for rain particles.
	* Set 1.0 for no refraction.
	*/
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Index Of Refraction")
//	float RainIndexOfRefraction = 0.5f;

	/** Mask Hardness for snow particles. Controls how 'soft' they look. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain", DisplayName = "Rain Mask Hardness", meta = (UIMin = "0.0", UIMax = "1.0"))
	float RainMaskHardness = 0.5f;

	/**
	* Max number of rain splash particles to spawn per second.
	* Value is multiplied by "Rain Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Spawn Rate Max", meta = (UIMin = "100.0", UIMax = "10000.0"))
	float RainSplashSpawnRateMax = 2000.0f;


	/**
	* Max number of rain splash particles to spawn per second.
	* Value is multiplied by "Rain Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Spawn Rate Max GPU", meta = (EditCondition = "bEnableGPUPrecipitation && bEnableOcclusionCapture", UIMin = "10000.0", UIMax = "200000.0"))
	float RainSplashSpawnRateMaxGPU = 100000.0f;

	/**
	* Index of Refraction for rain splash particles.
	* Set 1.0 for no refraction.
	*/
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rain Splash", DisplayName = "Rain Splash Index Of Refraction")
//	float RainSplashIndexOfRefraction = 1.1f;

	/**
	* Max number of snow particles to spawn per second.
	* Value is multiplied by "Snow Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Spawn Rate Max (CPU)", meta = (UIMin = "100.0", UIMax = "10000.0"))
	float SnowSpawnRateMaxCPU = 1000.0f;

	/**
	* Max number of snow particles to spawn per second.
	* Value is multiplied by "Snow Amount" value in weather preset.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Spawn Rate Max (GPU)", meta = (EditCondition = "bEnableGPUPrecipitation && bEnableOcclusionCapture", UIMin = "10000.0", UIMax = "200000.0"))
	float SnowSpawnRateMaxGPU = 100000.0f;

	/** Scale Factor by distance for snow particles (for better visibility). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Distance Scale Factor")
	float SnowDistanceScaleFactor = 2.0f;

	/**
	* Sets the amount of Camera Motion Alignment effect.
	* Set 0 to turn it off.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Camera Motion Alignment Scale", meta = (UIMin = "0.0", UIMax = "1.0"))
	float SnowCameraMotionAlignmentScale = 0.4f;

	/**
	* Elongation (stretching) scale based on velocity of a snow particle.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Velocity Elongation Scale", meta = (UIMin = "0.0", UIMax = "2.0"))
	float SnowVelocityElongationScale = 1.25f;

	/** Mask Hardness for snow particles. Controls how 'soft' they look. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Snow", DisplayName = "Snow Mask Hardness", meta = (UIMin = "0.0", UIMax = "1.0"))
	float SnowMaskHardness = 0.5f;



	/** Desc. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* LightningsParametersRT;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Weather FX|Lightnings", DisplayName = "Lightning Parameters", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	TArray<FSkyCreatorLightningParameters> LightningParameters;

	FTimerHandle LightningIntervalTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float CurrentLightningInterval;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Weather FX|Lightnings", DisplayName = "Last Lightning Position", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector LastLightningPosition;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Available Positions", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	TArray<FVector> LightningAvailablePositions;

	/** Desc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Current Index", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	int32 LightningCurrentIndex = 0;

	/** Enables sampling Volumetric Clouds to find a Lightning spawn position. Currently is a bit expensive to use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Sample Cloud Density")
	bool bSampleCloudDensity = true;

	/** Number of samples to find a Lightning spawn position from Volumetric Clouds. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Max Samples", meta = (EditCondition = "bSampleCloudDensity", UIMin = "0.0", ClampMin = "0.0", UIMax = "10.0"))
	int32 LightningMaxSamples = 4;

	/** Inner radius for a random Lightning spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Spawn Inner Radius")
	float LightningSpawnInnerRadius = 10.0f;

	/** Outer radius for a random Lightning spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Spawn Outer Radius")
	float LightningSpawnOuterRadius = 25.0f;

	/** Random Degree In Cone Max. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Random Degree In Cone Max")
	float LightningRandomDegreeInConeMax = 45.0f;

	/** Lightning Bolt Emissive Scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Bolt Emissive Scale")
	float LightningBoltEmissiveScale = 10.0f;

	FTimerHandle LightningFlashFadeTimerHandle;

	/** Update Rate of the Lightning Flash Fade. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Flash Fade Update Rate", meta = (ClampMin = "0.0", ClampMax = "120.0", UIMin = "30.0", UIMax = "120.0"))
	float LightningFlashFadeUpdateRate = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float LightningFlashFadeDelta = 0;

	/** Lightning Flash Emissive Scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Flash Emissive Scale")
	float LightningFlashEmissiveScale = 100.0f;

	/** Affects the scale of reflection injected to sky light. When set to 0, Lightning Flashes are not illuminating the world.. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Flash Emissive Reflection Scale")
	float LightningFlashEmissiveReflectionScale = 0.1f;

	/** Lightning Flash Radius Scale. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Flash Radius Scale")
	float LightningFlashRadiusScale = 1.0f;

	/** Lightning Flash Fade Speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Lightnings", DisplayName = "Lightning Flash Fade Speed", meta = (UIMin = "0.01", UIMax = "1.0"))
	float LightningFlashFadeSpeed = 0.5f;


	/** Camera Fade distance for precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Distance")
	float RainbowDistance = 20000.0f;

	/** Depth Fade distance for precipitation particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weather FX|Rainbow", DisplayName = "Rainbow Depth Fade Distance")
	float RainbowDepthFadeDistance = 50000.0f;



	/** Cutoff World Height of the all Material FX. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX", DisplayName = "Cutoff World Height")
	float MaterialFXCutoffWorldHeight = -100.0f;

	/** Cutoff Softness of the all Material FX. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX", DisplayName = "Cutoff Softness")
	float MaterialFXCutoffSoftness = 100.0f;

	/** Slope Angle for the Wetness effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Wetness", DisplayName = "Wetness Slope Angle", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float WetnessSlopeAngle = 0.0f;

	/** Slope Smoothness for the Wetness effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Wetness", DisplayName = "Wetness Slope Smoothness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WetnessSlopeSmoothness = 0.5f;


	/** Scale of the Puddles Mask in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Puddles", DisplayName = "Puddles Mask Scale", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "10000.0"))
	float PuddlesMaskScale = 2000.0f;

	/** Roughness of the Puddles effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Puddles", DisplayName = "Puddles Roughness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PuddlesRoughness = 0.02f;

	/** Slope Angle for the Puddles effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Puddles", DisplayName = "Puddles Slope Angle", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float PuddlesSlopeAngle = 0.75f;

	/** Slope Smoothness for the Puddles effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Puddles", DisplayName = "Puddles Slope Smoothness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PuddlesSlopeSmoothness = 0.5f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Enable Rain Ripples Solver", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	bool bEnableRainRipplesSolver = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Pause Rain Ripples Solver")
	bool bPauseRainRipplesSolver = false;

	FTimerHandle RainRipplesTimerHandle;

	/** Update Rate of the Rain Ripples Solver. Higher values increases simulation speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Update Rate", meta = (ClampMin = "0.0", ClampMax = "120.0", UIMin = "30.0", UIMax = "120.0"))
	float RainRipplesUpdateRate = 60.0f;

	/** Scale of the Rain Ripples in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Scale", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "1000.0"))
	float RainRipplesScale = 200.0f;

	/** Max density of Rain Ripples when the amount equals "1". */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Rain Ripples Max Density", meta = (ClampMin = "0.0", UIMin = "2.0", UIMax = "6.0"))
	float RainRipplesMaxDensity = 6.0f;

	/** Scale of the Wind Ripples in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", DisplayName = "Wind Ripples Scale", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "1000.0"))
	float WindRipplesScale = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float RainRipplesSolverDelta = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	int32 RainRipplesSolverHeightState = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInterface* RainRipplesPropagation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInstanceDynamic* RainRipplesPropagationMID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInterface* RainRipplesForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInterface* RainRipplesNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UMaterialInstanceDynamic* RainRipplesNormalMID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* RainRipplesPropagationFrame0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* RainRipplesPropagationFrame1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* RainRipplesPropagationFrame2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material FX|Ripples", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	UTextureRenderTarget2D* RainRipplesNormalRT;


	/** Scale of the Snow Mask in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Mask Scale", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "10000.0"))
	float SnowMaskScale = 500.0f;

	/** Scale of the Snow Material in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Scale", meta = (ClampMin = "1.0", UIMin = "100.0", UIMax = "4000.0"))
	float SnowScale = 500.0f;

	/** Roughness of the Snow Material. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Roughness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowRoughness = 0.4f;

	/** Scale of the Snow Sparkles effect in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Sparkles Scale", meta = (ClampMin = "1.0", UIMin = "10.0", UIMax = "50.0"))
	float SnowSparklesScale = 25.0f;

	/** Roughness of the Snow Sparkles effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Sparkles Roughness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowSparklesRoughness = 0.25f;

	/** Slope Angle for the Snow effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Slope Angle", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float SnowSlopeAngle = 0.5f;

	/** Slope Smoothness for the Snow effect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material FX|Snow", DisplayName = "Snow Slope Smoothness", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SnowSlopeSmoothness = 0.0f;



	/** Whether to enable Wind. Affects Volumetric Clouds and Weather FX particles. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wind", DisplayName = "Enable Wind")
	bool bEnableWind = true;

	/**
	* If set to true, Cloud & Weather FX Wind is controlled by either static values or by separate controller.
	* If set to false, Cloud & Weather FX Wind will read values from Weather Presets.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wind", DisplayName = "Independent Wind Control")
	bool bIndependentWindControl = true;

	/** Editor Independent Wind Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wind", DisplayName = "Editor Independent Wind Settings", meta = (ShowOnlyInnerProperties, EditCondition = "bIndependentWindControl"))
	FSkyCreatorWindSettings EditorIndependentWindSettings;

	/** Skew clouds towards Low Cloud Wind Direction. Affected by Low Cloud Wind Speed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Interp, Category = "Wind|Volumetric Clouds", DisplayName = "Cloud Wind Skew Amount", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "1.0"))
	float CloudWindSkewAmount = 0.25f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Wind", DisplayName = "Cloud Wind Offset", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CloudWindOffset = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Wind", DisplayName = "Cloud Wind Skew Direction", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CloudWindSkewDirection = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Wind", DisplayName = "Cloud Wind Skew Force", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	float CloudWindSkewForce = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Wind", DisplayName = "Cloud Noise Shape Wind Offset", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CloudNoiseShapeWindOffset = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Wind", DisplayName = "Cloud Noise Detail Wind Offset", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	FVector CloudNoiseDetailWindOffset = FVector(0, 0, 0);



	/** Whether to use built-in Exposure Settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Post Process", DisplayName = "Use Exposure Settings")
	bool bUseExposureSettings = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Post Process", DisplayName = "Extend Default Luminance Range", meta = (EditCondition = "bShowDebugVariables", EditConditionHides))
	bool bExtendDefaultLuminanceRange;

	/**
	* Priority of the Exposure component. It will override only Exposure settings from Post Process Volumes with lower priority,
	* unless additional Post Process settings modified on this component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (EditCondition = "bUseExposureSettings", DisplayName = "Exposure Priority"))
	float PostProcessPriority = 0.0f;

	/** Luminance computation method. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (EditCondition = "bUseExposureSettings", DisplayName = "Metering Mode"))
	TEnumAsByte<enum EAutoExposureMethod> ExposureMethod;

	/**
	* Exposure compensation based on the scene EV100.
	* Used to calibrate the final exposure differently depending on the average scene luminance.
	* 0: no adjustment, -1:2x darker, -2:4x darker, 1:2x brighter, 2:4x brighter, ...
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (EditCondition = "bUseExposureSettings", DisplayName = "Exposure Compensation Curve"))
	class UCurveFloat* ExposureBiasCurve = nullptr;

	/**
	* Exposure metering mask. Bright spots on the mask will have high influence on auto-exposure metering
	* and dark spots will have low influence.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (EditCondition = "bUseExposureSettings", DisplayName = "Exposure Metering Mask"))
	class UTexture* ExposureMeterMask = nullptr;

	/**
	* Auto-Exposure minimum adaptation. Eye Adaptation is disabled if Min = Max.
	* Auto-exposure is implemented by choosing an exposure value for which the average luminance generates a pixel brightness equal to the Constant Calibration value.
	* The Min/Max are expressed in pixel luminance (cd/m2).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "-10.0", UIMax = "20.0", EditCondition = "bUseExposureSettings && !bExtendDefaultLuminanceRange", DisplayName = "Min Brightness"))
	float ExposureMinBrightness = 0.02f;

	/**
	* Auto-Exposure maximum adaptation. Eye Adaptation is disabled if Min = Max.
	* Auto-exposure is implemented by choosing an exposure value for which the average luminance generates a pixel brightness equal to the Constant Calibration value.
	* The Min/Max are expressed in pixel luminance (cd/m2).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "-10.0", UIMax = "20.0", EditCondition = "bUseExposureSettings && !bExtendDefaultLuminanceRange", DisplayName = "Max Brightness"))
	float ExposureMaxBrightness = 20.0f;

	/**
	* Auto-Exposure minimum adaptation. Eye Adaptation is disabled if Min = Max.
	* Auto-exposure is implemented by choosing an exposure value for which the average luminance generates a pixel brightness equal to the Constant Calibration value.
	* The Min/Max are expressed in EV100 when using ExtendDefaultLuminanceRange (see project settings).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "-10.0", UIMax = "20.0", EditCondition = "bUseExposureSettings && bExtendDefaultLuminanceRange", DisplayName = "Min EV100"))
	float ExposureMinEV100 = -6.0f;

	/**
	* Auto-Exposure maximum adaptation. Eye Adaptation is disabled if Min = Max.
	* Auto-exposure is implemented by choosing an exposure value for which the average luminance generates a pixel brightness equal to the Constant Calibration value.
	* The Min/Max are expressed in EV100 when using ExtendDefaultLuminanceRange (see project settings).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "-10.0", UIMax = "20.0", EditCondition = "bUseExposureSettings && bExtendDefaultLuminanceRange", DisplayName = "Max EV100"))
	float ExposureMaxEV100 = 20.0f;

	/** In F-stops per second, should be >0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "0.02", UIMax = "20.0", EditCondition = "bUseExposureSettings", DisplayName = "Speed Up"))
	float ExposureSpeedUp = 4.0f;

	/** In F-stops per second, should be >0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", meta = (ClampMin = "0.02", UIMax = "20.0", EditCondition = "bUseExposureSettings", DisplayName = "Speed Down"))
	float ExposureSpeedDown = 4.0f;

	/**
	* The eye adaptation will adapt to a value extracted from the luminance histogram of the scene color.
	* The value is defined as having x percent below this brightness. Higher values give bright spots on the screen more priority
	* but can lead to less stable results. Lower values give the medium and darker values more priority but might cause burn out of
	* bright spots.
	* >0, <100, good values are in the range 70 .. 80
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "bUseExposureSettings", DisplayName = "Low Percent"))
	float ExposureLowPercent = 70.0f;

	/**
	* The eye adaptation will adapt to a value extracted from the luminance histogram of the scene color.
	* The value is defined as having x percent below this brightness. Higher values give bright spots on the screen more priority
	* but can lead to less stable results. Lower values give the medium and darker values more priority but might cause burn out of
	* bright spots.
	* >0, <100, good values are in the range 80 .. 95
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (ClampMin = "0.0", ClampMax = "100.0", EditCondition = "bUseExposureSettings", DisplayName = "High Percent"))
	float ExposureHighPercent = 90.0f;

	/** Histogram Min value. Expressed in Log2(Luminance). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (UIMin = "-16", UIMax = "0.0", EditCondition = "bUseExposureSettings && !bExtendDefaultLuminanceRange", DisplayName = "Histogram Log Min"))
	float ExposureHistogramLogMin = -16.0f;

	/** Histogram Max value. Expressed in Log2(Luminance). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (UIMin = "0.0", UIMax = "16.0", EditCondition = "bUseExposureSettings && !bExtendDefaultLuminanceRange", DisplayName = "Histogram Log Max"))
	float ExposureHistogramLogMax = 4.0f;

	/** Histogram Min value. Expressed in EV100 when using ExtendDefaultLuminanceRange (see project settings) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (UIMin = "-16", UIMax = "0.0", EditCondition = "bUseExposureSettings && bExtendDefaultLuminanceRange", DisplayName = "Histogram Min EV100"))
	float ExposureHistogramMinEV100 = -12.0f;

	/** Histogram Max value. Expressed in EV100 when using ExtendDefaultLuminanceRange (see project settings) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process", AdvancedDisplay, meta = (UIMin = "0.0", UIMax = "16.0", EditCondition = "bUseExposureSettings && bExtendDefaultLuminanceRange", DisplayName = "Histogram Max EV100"))
	float ExposureHistogramMaxEV100 = 20.0f;


	UPROPERTY(BlueprintAssignable)
	FOnLightningStrike OnLightningStrike;



	UFUNCTION(Category = "Sky Creator|Weather")
	FRotator GetSunPosition(float Time);

	UFUNCTION(Category = "Sky Creator|Weather")
	FRotator GetMoonPosition(float Time);

	UFUNCTION(Category = "Sky Creator|Weather")
	FRotator GetStarMapRotation() const;


	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetSkyAtmosphereSettings(FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetVolumetricCloudSettings(FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetBackgroundCloudSettings(FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetSkyLightSettings(FSkyCreatorSkyLightSettings InSkyLightSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetSunLightSettings(FSkyCreatorSunLightSettings InSunLightSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetMoonLightSettings(FSkyCreatorMoonLightSettings InMoonLightSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetExponentialHeightFogSettings(FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetStarMapSettings(FSkyCreatorStarMapSettings InStarMapSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetWeatherFXSettings(FSkyCreatorWeatherFXSettings InWeatherFXSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetWeatherMaterialFXSettings(FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetWindSettings(FSkyCreatorWindSettings InWindSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetWindIndependentSettings(FSkyCreatorWindSettings InWindSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetPostProcessSettings(FSkyCreatorPostProcessSettings InPostProcessSettings);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void SetWeatherSettings(FSkyCreatorWeatherSettings InWeatherSettings);


	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpSkyAtmosphereSettings(FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettingsA, FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpVolumetricCloudSettings(FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettingsA, FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpBackgroundCloudSettings(FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettingsA, FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpSkyLightSettings(FSkyCreatorSkyLightSettings InSkyLightSettingsA, FSkyCreatorSkyLightSettings InSkyLightSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpSunLightSettings(FSkyCreatorSunLightSettings InSunLightSettingsA, FSkyCreatorSunLightSettings InSunLightSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpMoonLightSettings(FSkyCreatorMoonLightSettings InMoonLightSettingsA, FSkyCreatorMoonLightSettings InMoonLightSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpExponentialHeightFogSettings(FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettingsA, FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpStarMapSettings(FSkyCreatorStarMapSettings InStarMapSettingsA, FSkyCreatorStarMapSettings InStarMapSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWeatherFXSettings(FSkyCreatorWeatherFXSettings InWeatherFXSettingsA, FSkyCreatorWeatherFXSettings InWeatherFXSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWeatherMaterialFXSettings(FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettingsA, FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWindSettings(FSkyCreatorWindSettings InWindSettingsA, FSkyCreatorWindSettings InWindSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWindIndependentSettings(FSkyCreatorWindSettings InWindSettingsA, FSkyCreatorWindSettings InWindSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpPostProcessSettings(FSkyCreatorPostProcessSettings InPostProcessSettingsA, FSkyCreatorPostProcessSettings InPostProcessSettingsB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWeatherSettings(FSkyCreatorWeatherSettings InWeatherSettingsA, FSkyCreatorWeatherSettings InWeatherSettingsB, float Alpha);


	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpWetnessAmount(float WetnessAmountA, float WetnessAmountB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpPuddlesAmount(float PuddlesAmountA, float PuddlesAmountB, float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Weather")
	virtual void LerpSnowAmount(float SnowAmountA, float SnowAmountB, float Alpha);


	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorSkyAtmosphereSettings GetSkyAtmosphereSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorVolumetricCloudSettings GetVolumetricCloudSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorBackgroundCloudSettings GetBackgroundCloudSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorSkyLightSettings GetSkyLightSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorSunLightSettings GetSunLightSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorMoonLightSettings GetMoonLightSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorExponentialHeightFogSettings GetExponentialHeightFogSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorStarMapSettings GetStarMapSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorWeatherFXSettings GetWeatherFXSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorWeatherMaterialFXSettings GetWeatherMaterialFXSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorWindSettings GetWindSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorPostProcessSettings GetPostProcessSettings() const;

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Weather")
	FSkyCreatorWeatherSettings GetWeatherSettings() const;


	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Time")
	void SetTime(float InTime);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Time")
	float GetTime() const;

	UFUNCTION(BlueprintCallable, Category = "Sky Creator")
	void SetSunSimplePositionSettings(const float InSunriseTime = 6.5f, const float InSunsetTime = 19.5f, const float InSunElevation = 55.0f, const float InSunAzimuth = 270.0f);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator")
	void SetMoonSimplePositionSettings(const float InMoonriseTime = 18.0f, const float InMoonsetTime = 8.0f, const float InMoonElevation = 45.0f, const float InMoonAzimuth = 270.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Sky Creator")
	void SetRealPositionSettings(const float InLatitude = 51.509865f, const float InLongitude = -0.118092f, const float InTimeZone = 0.0f, const bool InbDaylightSavingTime = false, const int InYear = 2022, const int InMonth = 5, const int InDay = 10);

//	UFUNCTION(BlueprintCallable, Category = "Sky Creator|Moon")
//	void SetMoonPhase(float InMoonPhase);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Moon")
	float GetMoonPhase() const;

	UFUNCTION(BlueprintCallable, Category = "Sky Creator")
	void RealtimeTimeOfDay(float DeltaSeconds, float DayCycleDuration);

	UFUNCTION(BlueprintCallable, Category = "Sky Creator")
	void SpawnLightningStrike(FVector LightningPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sky Creator")
	FVector GetLastLightningPosition();

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	float GetCloudDensityAtPosition(FVector Position);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	bool FindLightningPosition(FVector Position, FVector& OutPosition);

	UFUNCTION(Category = "Sky Creator")
	void UpdateSettingsSequencer();

	
	void SetSunMoonPosition(float InTime);
	void UpdateSunMoonIntensity(float InTime);
	void RealtimeVolumetricCloudWind(const float DeltaTime);
	void RipplesSolverPropagation();
	void RipplesSolverClearRT();
	void CheckOcclusion();
	void CaptureOcclusion(FVector CaptureLocation);
	void WriteLightningParameters(FVector InLightningPosition, float InLightningRadius, FLinearColor InLightningColor);
	void LightningFlashFade();
	void MakeLightningStrike();
	UTextureRenderTarget2D* RipplesSolverGetFrame(const int32 Index) const;
	UTextureRenderTarget2D* RipplesSolverGetLastFrame(const int32 Index, const int32 NumFramesOld) const;
	FVector GetCurrentCameraPosition() const;


	UFUNCTION(CallInEditor, Category = "Sky Creator|General")
	void SetEditorTimeOfDay(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|General")
	void SetEditorWeatherPreset(USkyCreatorWeatherPreset* NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|General")
	void SetEditorWeatherSettings(FSkyCreatorWeatherSettings NewValue);
	
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunriseTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunsetTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunDawnOffsetTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunDuskOffsetTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunElevation(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunAzimuth(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetSunMinAngleAtDawnDusk(float NewValue);

	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetMoonriseTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetMoonsetTime(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetMoonElevation(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetMoonAzimuth(float NewValue);

	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetLatitude(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetLongitude(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetTimeZone(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position", meta = (DisplayName = "Set Daylight Saving Time"))
	void SetbDaylightSavingTime(bool NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetYear(int32 NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetMonth(int32 NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun & Moon Position")
	void SetDay(int32 NewValue);

	UFUNCTION(CallInEditor, Category = "Sky Creator|Volumetric Clouds")
	void SetLayerBottomAltitude(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Volumetric Clouds")
	void SetLayerHeight(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Volumetric Clouds")
	void SetCloudMapOffset(FVector2D NewValue);

//	UFUNCTION(CallInEditor, Category = "Sky Creator|Sky Light", meta = (DisplayName = "Set Real Time Capture"))
//	void SetbRealTimeCapture(bool NewValue);
	
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun Light")
	void SetSunIntensity(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun Light")
	void SetSunDiskSize(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Sun Light")
	void SetSunAtmosphereDiskColorScale(FLinearColor NewValue);

	UFUNCTION(CallInEditor, Category = "Sky Creator|Moon Light")
	void SetMoonIntensity(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Moon Light")
	void SetMoonDiskSize(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Moon Light")
	void SetMoonRotation(float NewValue);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Sky Creator|Moon Light")
	void SetMoonPhase(float NewValue);
	UFUNCTION(CallInEditor, Category = "Sky Creator|Moon Light")
	void SetMoonAtmosphereDiskColorScale(FLinearColor NewValue);

	UFUNCTION(CallInEditor, Category = "Sky Creator|Star Map")
	void SetStarMapAdditionalRotation(FVector NewValue);

private:

	void SelectVolumetricCloudMaterial();
//	void RerunConstructionScript();
	void SetComponentsSettings();
	void SetMPCSettings();
	void SetWeatherFXStaticSettings();
	void SetWeatherMaterialFXStaticSettings();
	void UpdateSettings();

	UFUNCTION(Category = "Sky Creator|Time")
	void OnRep_UpdateTime();

	UFUNCTION(Category = "Sky Creator|Weather")
	void OnRep_UpdateWeather();
};
