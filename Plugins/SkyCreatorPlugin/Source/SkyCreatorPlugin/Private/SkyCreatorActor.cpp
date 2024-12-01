// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorActor.h"
#include "Components/SceneComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Canvas.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Launch/Resources/Version.h"

// Sets default values
ASkyCreator::ASkyCreator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	//Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// Find Textures
		ConstructorHelpers::FObjectFinder<UTexture2D> BillboardIcon;
		ConstructorHelpers::FObjectFinder<UTexture2D> StarMapTexture;
		ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> OcclusionRT;
		ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RainRipplesNormalRT;
		ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> LightningParametersRT;
		// Find Materials
		ConstructorHelpers::FObjectFinder<UMaterialInterface> VolumetricCloudMaterial;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> VolumetricCloudDensitySampleMaterial;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> SkySphereMaterial;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> RainRipplesPropagation;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> RainRipplesForce;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> RainRipplesNormal;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> PostProcessWorldHeight;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> DirectionalLightFunction;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> SunLightFunction;
		ConstructorHelpers::FObjectFinder<UMaterialInterface> MoonLightFunction;
		// Find misc
//		ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem;
		ConstructorHelpers::FObjectFinder<UStaticMesh> Compass;
		ConstructorHelpers::FObjectFinder<UStaticMesh> SkySphere;
		ConstructorHelpers::FObjectFinder<USkyCreatorWeatherPreset> DefaultWeatherPreset;
		ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> CommonMPC;
		ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> VolumetricCloudsMPC;

		FConstructorStatics()
			: BillboardIcon(TEXT("Texture2D'/SkyCreatorPlugin/Textures/Icons/T_SkyCreator_BillboardIcon'"))
			, StarMapTexture(TEXT("Texture2D'/SkyCreatorPlugin/Textures/SkySphere/TC_SC_StarMap_4K'"))
			, OcclusionRT(TEXT("Texture2D'/SkyCreatorPlugin/Textures/Occlusion/RT_SC_WorldHeightmap'"))
			, RainRipplesNormalRT(TEXT("Texture2D'/SkyCreatorPlugin/Textures/WeatherMaterialFX/RT_SC_RainRipples_Normal'"))
			, LightningParametersRT(TEXT("Texture2D'/SkyCreatorPlugin/Textures/WeatherFX/RT_SC_LightningParameters'"))

			, VolumetricCloudMaterial(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1'")) // Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1'
			, VolumetricCloudDensitySampleMaterial(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/VolumetricClouds/MI_SC_VolumetricCloud_DensitySample.MI_SC_VolumetricCloud_DensitySample'"))
			, SkySphereMaterial(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/SkySphere/MI_SC_SkySphere.MI_SC_SkySphere'"))
			, RainRipplesPropagation(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/WeatherMaterialFX/M_SC_RainRipples_Propagation.M_SC_RainRipples_Propagation'"))
			, RainRipplesForce(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/WeatherMaterialFX/M_SC_RainRipples_Force.M_SC_RainRipples_Force'"))
			, RainRipplesNormal(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/WeatherMaterialFX/M_SC_RainRipples_Normal.M_SC_RainRipples_Normal'"))
			, PostProcessWorldHeight(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/Occlusion/PP_SC_WorldHeight.PP_SC_WorldHeight'"))
			, DirectionalLightFunction(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/Common/M_SC_DirectionalLightFunction.M_SC_DirectionalLightFunction'"))
			, SunLightFunction(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/Common/M_SC_SunLightFunction.M_SC_SunLightFunction'"))
			, MoonLightFunction(TEXT("MaterialInterface'/SkyCreatorPlugin/Materials/Common/M_SC_MoonLightFunction.M_SC_MoonLightFunction'"))

//			, NiagaraSystem(TEXT("NiagaraSystem'/SkyCreatorPlugin/Effects/SC_WeatherFX_Precipitation_System'"))
			, Compass(TEXT("StaticMesh'/SkyCreatorPlugin/Meshes/SM_SC_Compass'"))
			, SkySphere(TEXT("StaticMesh'/SkyCreatorPlugin/Meshes/SM_SC_SkySphere'"))
			, DefaultWeatherPreset(TEXT("DataAsset'/SkyCreatorPlugin/WeatherPresets/Weather_Default'"))
			, CommonMPC(TEXT("MaterialParameterCollection'/SkyCreatorPlugin/Materials/Common/MPC_SC_Common'"))
			, VolumetricCloudsMPC(TEXT("MaterialParameterCollection'/SkyCreatorPlugin/Materials/VolumetricClouds/MPC_SC_VolumetricClouds'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
//	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 100.0f;

	//Set default textures
	StarMapTexture = ConstructorStatics.StarMapTexture.Object;
	OcclusionRenderTarget = ConstructorStatics.OcclusionRT.Object;
	RainRipplesNormalRT = ConstructorStatics.RainRipplesNormalRT.Object;

	//Set default materials
	CurrentVolumetricCloudMaterial = ConstructorStatics.VolumetricCloudMaterial.Object;
	VolumetricCloudDensitySampleMaterial = ConstructorStatics.VolumetricCloudDensitySampleMaterial.Object;
	SkySphereMaterial = ConstructorStatics.SkySphereMaterial.Object;
	RainRipplesPropagation = ConstructorStatics.RainRipplesPropagation.Object;
	RainRipplesForce = ConstructorStatics.RainRipplesForce.Object;
	RainRipplesNormal = ConstructorStatics.RainRipplesNormal.Object;

	//Set default misc
	EditorWeatherPreset = ConstructorStatics.DefaultWeatherPreset.Object;
	CommonMPC = ConstructorStatics.CommonMPC.Object;
	VolumetricCloudsMPC = ConstructorStatics.VolumetricCloudsMPC.Object;

	//Set root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	Root->SetMobility(EComponentMobility::Type::Movable);
	RootComponent = Root;

#if WITH_EDITOR
	//Set billboard
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard Component"));
	if (Billboard)
	{
		Billboard->SetMobility(EComponentMobility::Type::Movable);
		Billboard->SetupAttachment(RootComponent);
		Billboard->Sprite = ConstructorStatics.BillboardIcon.Object;
		Billboard->bIsScreenSizeScaled = true;
		Billboard->ScreenSize = 0.001f;
		Billboard->SetRelativeLocation(FVector(0, 0, 100.0f));
	}

	Compass = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("Compass Mesh Component"));
	if (Compass)
	{
		Compass->SetMobility(EComponentMobility::Type::Movable);
		Compass->SetupAttachment(RootComponent);
		Compass->SetCollisionProfileName("NoCollision", false);
		Compass->SetStaticMesh(ConstructorStatics.Compass.Object);
		Compass->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
		Compass->SetHiddenInGame(true);
		Compass->SetCastShadow(false);
		Compass->bCastStaticShadow = false;
	}
#endif

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("Sky Atmosphere Component"));
	if (SkyAtmosphere)
	{
		SkyAtmosphere->SetMobility(SkyAtmosphereMobility);
		SkyAtmosphere->SetupAttachment(RootComponent);
		SkyAtmosphere->AerialPerspectiveStartDepth = 0.01f;
	}

	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("Volumetric Cloud Component"));
	if (VolumetricCloud)
	{
		VolumetricCloud->SetMobility(EComponentMobility::Type::Movable);
		VolumetricCloud->SetupAttachment(RootComponent);
	}

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("Sky Light Component"));
	if (SkyLight)
	{
		SkyLight->SetMobility(SkyLightMobility);
		SkyLight->SetupAttachment(RootComponent);
	}

	// little fix for 4.26-4.27 which has visual 'black sky reflection' bug capturing skylight over time slices when having another Scene Capture component which has disabled flags for Atmosphere, Fog, etc.
#if ENGINE_MAJOR_VERSION == 4
//	#if ENGINE_MINOR_VERSION == 26
		bSkyLightCaptureTimeSlice = false;
//	#endif
#endif

	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun Light Component"));
	if (SunLight)
	{
		SunLight->SetMobility(SunLightMobility);
		SunLight->SetupAttachment(RootComponent);
		SunLight->SetAtmosphereSunLight(true);
		SunLight->SetAtmosphereSunLightIndex(0);
		SunLight->bCastCloudShadows = true;
//		SunLight->bPerPixelAtmosphereTransmittance = true;
		SunLight->CloudShadowExtent = 25.0f;
		SunLight->LightFunctionMaterial = ConstructorStatics.SunLightFunction.Object;
//		SunLight->LightFunctionFadeDistance = 0.0f;
//		SunLight->DisabledBrightness = 1.0f;
#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
		SunLight->ForwardShadingPriority = 1;
#endif
#endif
	}

	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Moon Light Component"));
	if (MoonLight)
	{
		MoonLight->SetMobility(MoonLightMobility);
		MoonLight->SetupAttachment(RootComponent);
		MoonLight->SetAtmosphereSunLight(true);
		MoonLight->SetAtmosphereSunLightIndex(1);
		MoonLight->bCastCloudShadows = true;
//		MoonLight->bPerPixelAtmosphereTransmittance = true;
		MoonLight->CloudShadowExtent = 25.0f;
		MoonLight->LightFunctionMaterial = ConstructorStatics.MoonLightFunction.Object;
//		MoonLight->LightFunctionFadeDistance = 0.0f;
//		MoonLight->DisabledBrightness = 1.0f;

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
		MoonLight->ForwardShadingPriority = 0;
#endif
#endif
	}

	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Exponential Height Fog Component"));
	if (ExponentialHeightFog)
	{
		ExponentialHeightFog->SetMobility(EComponentMobility::Type::Movable);
		ExponentialHeightFog->SetupAttachment(RootComponent);

		if (bEnableExponentialHeightFog)
		{
			ExponentialHeightFog->SetVisibility(true);
			ExponentialHeightFog->SetHiddenInGame(false);
		}
		else 
		{
			ExponentialHeightFog->SetVisibility(false);
			ExponentialHeightFog->SetHiddenInGame(true);
		}
	}

	SkySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sky Sphere Component"));
	if (SkySphere)
	{
		SkySphere->SetMobility(EComponentMobility::Type::Movable);
		SkySphere->SetupAttachment(RootComponent);
		SkySphere->SetCollisionProfileName("NoCollision", false);
		SkySphere->SetStaticMesh(ConstructorStatics.SkySphere.Object);
		SkySphere->SetCastShadow(false);
		SkySphere->SetRelativeScale3D(FVector(SkySphereRadius, SkySphereRadius, SkySphereRadius));
	}

	OcclusionCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Occlusion Capture Component"));
	if (OcclusionCapture)
	{
		OcclusionCapture->SetupAttachment(RootComponent);
		//OcclusionCapture->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		OcclusionCapture->SetMobility(EComponentMobility::Type::Movable);
		OcclusionCapture->ProjectionType = ECameraProjectionMode::Orthographic;
		OcclusionCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
		OcclusionCapture->bCaptureEveryFrame = false;
		OcclusionCapture->bCaptureOnMovement = false;
		OcclusionCapture->bAlwaysPersistRenderingState = true;
		OcclusionCapture->ProfilingEventName = FString(TEXT("Sky Creator Occlusion Capture"));
		OcclusionCapture->AddOrUpdateBlendable(ConstructorStatics.PostProcessWorldHeight.Object);
		// General Show Flags
		OcclusionCapture->ShowFlags.AntiAliasing = false;
		OcclusionCapture->ShowFlags.Atmosphere = false;
		OcclusionCapture->ShowFlags.Decals = false;
		OcclusionCapture->ShowFlags.Fog = false;
		OcclusionCapture->ShowFlags.Particles = false;
		OcclusionCapture->ShowFlags.SkeletalMeshes = false;
		// Advanced Show Flags
		OcclusionCapture->ShowFlags.DeferredLighting = false;
		OcclusionCapture->ShowFlags.InstancedFoliage = false;
//		OcclusionCapture->ShowFlags.InstancedStaticMeshes = true;
		OcclusionCapture->ShowFlags.InstancedGrass = false;
		OcclusionCapture->ShowFlags.Paper2DSprites = false;
		OcclusionCapture->ShowFlags.TextRender = false;
		OcclusionCapture->ShowFlags.TemporalAA = false;
		// Post Processing Show Flags
		OcclusionCapture->ShowFlags.Bloom = false;
		OcclusionCapture->ShowFlags.EyeAdaptation = false;
		OcclusionCapture->ShowFlags.MotionBlur = false;
		OcclusionCapture->ShowFlags.ToneCurve = false;
		// Light Types Show Flags
		OcclusionCapture->ShowFlags.SkyLighting = false;
		// Lighting Components Show Flags
		OcclusionCapture->ShowFlags.AmbientOcclusion = false;
		OcclusionCapture->ShowFlags.DynamicShadows = false;
		// Lighting Features Show Flags
		OcclusionCapture->ShowFlags.AmbientCubemap = false;
		OcclusionCapture->ShowFlags.DistanceFieldAO = false;
		OcclusionCapture->ShowFlags.LightFunctions = false;
		OcclusionCapture->ShowFlags.LightShafts = false;
		OcclusionCapture->ShowFlags.ReflectionEnvironment = false;
		OcclusionCapture->ShowFlags.ScreenSpaceReflections = false;
		OcclusionCapture->ShowFlags.TexturedLightProfiles = false;
		OcclusionCapture->ShowFlags.VolumetricFog = false;
		// Hidden Show Flags
		OcclusionCapture->ShowFlags.Game = false;
		OcclusionCapture->ShowFlags.Lighting = false;
	}

	WeatherFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Weather FX Component"));
	if (WeatherFX)
	{
		WeatherFX->SetupAttachment(RootComponent);
//		WeatherFX->SetAsset(ConstructorStatics.NiagaraSystem.Object);
//		WeatherFX->BoundsScale = SkySphereRadius;
	}

	LightningsParametersRT = ConstructorStatics.LightningParametersRT.Object;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Post Process Component"));
	if (PostProcess)
	{
		PostProcess->SetMobility(EComponentMobility::Type::Movable);
		PostProcess->SetupAttachment(RootComponent);
		PostProcess->Priority = PostProcessPriority;
		PostProcess->Settings.bOverride_BloomIntensity = true;
		PostProcess->Settings.bOverride_BloomThreshold = true;
		PostProcess->Settings.bOverride_BloomSizeScale = true;
		PostProcess->Settings.bOverride_AutoExposureMethod = true;
		PostProcess->Settings.bOverride_AutoExposureLowPercent = true;
		PostProcess->Settings.bOverride_AutoExposureHighPercent = true;
		PostProcess->Settings.bOverride_AutoExposureMinBrightness = true;
		PostProcess->Settings.bOverride_AutoExposureMaxBrightness = true;
		PostProcess->Settings.bOverride_AutoExposureSpeedUp = true;
		PostProcess->Settings.bOverride_AutoExposureSpeedDown = true;
		PostProcess->Settings.bOverride_AutoExposureBias = true;
		PostProcess->Settings.bOverride_AutoExposureBiasCurve = true;
		PostProcess->Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = true;
		PostProcess->Settings.bOverride_AutoExposureMeterMask = true;
		PostProcess->Settings.bOverride_HistogramLogMin = true;
		PostProcess->Settings.bOverride_HistogramLogMax = true;
		PostProcess->Settings.bOverride_AutoExposureCalibrationConstant_DEPRECATED = true;
		PostProcess->Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = true;

		PostProcess->Settings.BloomSizeScale = 16.0f;

		// next value might get overwritten by r.DefaultFeature.AutoExposure
		static const auto VarDefaultAutoExposureExtendDefaultLuminanceRange = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.DefaultFeature.AutoExposure.ExtendDefaultLuminanceRange"));
		if (VarDefaultAutoExposureExtendDefaultLuminanceRange->GetValueOnAnyThread() != 0)
		{
			bExtendDefaultLuminanceRange = true;
		}
		else
		{
			bExtendDefaultLuminanceRange = false;
		}
	}
	
	EditorIndependentWindSettings.CloudWindDirection = 60.0f;
	EditorIndependentWindSettings.CloudWindSpeed = 4.0f;
	EditorIndependentWindSettings.CloudNoiseDetailWindSpeedVertical = 2.0f;
	EditorIndependentWindSettings.PrecipitationWindDirection = 60.0f;
	EditorIndependentWindSettings.PrecipitationWindSpeed = 4.0f;
/*
	SunPositionData.Elevation = 0.0;
	SunPositionData.CorrectedElevation = 0.0;
	SunPositionData.Azimuth = 0.0;
	SunPositionData.CelestialNoon = 0.0;
	SunPositionData.CelestialRiseTime = 0.0;
	SunPositionData.CelestialSetTime = 0.0;

	MoonPositionData.Elevation = 0.0;
	MoonPositionData.CorrectedElevation = 0.0;
	MoonPositionData.Azimuth = 0.0;
	MoonPositionData.CelestialNoon = 0.0;
	MoonPositionData.CelestialRiseTime = 0.0;
	MoonPositionData.CelestialSetTime = 0.0;*/

	if (LightningsParametersRT)
	{
		const int32 Size = LightningsParametersRT->SizeX;
		LightningParameters.Init( { FLinearColor(0, 0, 0, 1000), FLinearColor(0, 0, 0, 1000) }, Size);
	}

//	LightningsPositions.Init(FLinearColor(0, 0, 0, 1000), 32);
//	LightningsColors.Init(FLinearColor(0, 0, 0, 0), 32);
}

// Called when the game starts or when spawned
void ASkyCreator::BeginPlay()
{
	Super::BeginPlay();

	CaptureOcclusion(CameraLocationSnapped);
	SetMPCSettings();
	SetWeatherFXStaticSettings();
	SetWeatherSettings(WeatherSettings);
//	SetWeatherFXSettings(WeatherSettings.WeatherFXSettings);
//	SetWeatherMaterialFXStaticSettings();
//	SetWeatherMaterialFXSettings(WeatherSettings.WeatherMaterialFXSettings);
	RipplesSolverClearRT();
}

// Called every frame
void ASkyCreator::Tick(float DeltaTime)
{
	const UWorld* World = GetWorld();

	if (World)
	{
		if (World->WorldType == EWorldType::Editor)
		{
#if WITH_EDITOR
			EditorTick(DeltaTime);
#endif
		}
		else
		{
			Super::Tick(DeltaTime);
		}

		// if in Game and if not a server
/*		if (World->HasBegunPlay() && !UKismetSystemLibrary::IsDedicatedServer(this))
		{
			//if (bEnableRainRipplesSolver) RipplesSolverPropagation(DeltaTime);
			if (bEnableOcclusionCapture) CheckOcclusion();
		}
		// if in Editor
		else if (!World->HasBegunPlay())
		{
			//if (bEnableRainRipplesSolver) RipplesSolverPropagation(DeltaTime);
			if (bEnableOcclusionCapture) CheckOcclusion();
		}*/
	}

//	DrawLightningsCloudCoverage(DeltaTime);

//	LightningFlashFade(DeltaTime);

	if (USkyCreatorFunctionLibrary::IsApplicationForegroundNow())
	{
		// Client-side only effects
		if (!UKismetSystemLibrary::IsDedicatedServer(this))
		{
			CameraLocation = GetCurrentCameraPosition();
			if (bEnableOcclusionCapture)
			{
				CheckOcclusion();
			}

			RealtimeVolumetricCloudWind(DeltaTime);
		}

		// Run cloud wind only on server
		if (HasAuthority())
		{
//			RealtimeVolumetricCloudWind(DeltaTime);
		}

		// Setting up Timer Handle for Lightning intervals
		if (WeatherSettings.WeatherFXSettings.EnableLightnings)
		{
			if (!GetWorldTimerManager().IsTimerActive(LightningIntervalTimerHandle) && USkyCreatorFunctionLibrary::IsApplicationForegroundNow())
			{
				CurrentLightningInterval = UKismetMathLibrary::RandomFloatInRange(WeatherSettings.WeatherFXSettings.LightningSpawnIntervalMin, WeatherSettings.WeatherFXSettings.LightningSpawnIntervalMax);
				GetWorldTimerManager().SetTimer(LightningIntervalTimerHandle, this, &ASkyCreator::MakeLightningStrike, CurrentLightningInterval, false);
				//UKismetSystemLibrary::PrintString(this, "Setting Timer", true, false, FLinearColor::White, World->DeltaTimeSeconds);
			}
		}
		else
		{
			if (GetWorldTimerManager().TimerExists(LightningIntervalTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(LightningIntervalTimerHandle);
				//UKismetSystemLibrary::PrintString(this, "Clearing Timer", true, false, FLinearColor::White, World->DeltaTimeSeconds);
			}
		}

		// Setting up Timer Handle for Lightning Flash Fade
		if (!GetWorldTimerManager().IsTimerActive(LightningFlashFadeTimerHandle) && USkyCreatorFunctionLibrary::IsApplicationForegroundNow())
		{
			GetWorldTimerManager().SetTimer(LightningFlashFadeTimerHandle, this, &ASkyCreator::LightningFlashFade, LightningFlashFadeDelta, false);
			//UKismetSystemLibrary::PrintString(this, "Setting Timer", true, false, FLinearColor::White, World->DeltaTimeSeconds);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, World->GetDeltaSeconds(), FColor::White, FString::Printf(TEXT("Time Remaining: %f"), GetWorldTimerManager().GetTimerRemaining(LightningIntervalTimerHandle)));
		}

		// Setting up Timer Handle for Rain Ripples
		if (bEnableRainRipplesSolver)
		{
			if (!GetWorldTimerManager().IsTimerActive(RainRipplesTimerHandle) && USkyCreatorFunctionLibrary::IsApplicationForegroundNow())
			{
				if (!bPauseRainRipplesSolver)
				{
					GetWorldTimerManager().SetTimer(RainRipplesTimerHandle, this, &ASkyCreator::RipplesSolverPropagation, RainRipplesSolverDelta, false);
					//UKismetSystemLibrary::PrintString(this, "Setting Timer", true, false, FLinearColor::White, World->DeltaTimeSeconds);
				}
				else
				{
					GetWorldTimerManager().PauseTimer(RainRipplesTimerHandle);
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, World->GetDeltaSeconds(), FColor::White, FString::Printf(TEXT("Time Remaining: %f"), GetWorldTimerManager().GetTimerRemaining(LightningIntervalTimerHandle)));
			}
		}
		else
		{
			if (GetWorldTimerManager().TimerExists(RainRipplesTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(RainRipplesTimerHandle);
				//UKismetSystemLibrary::PrintString(this, "Clearing Timer", true, false, FLinearColor::White, World->DeltaTimeSeconds);
			}
			RipplesSolverClearRT();
		}

		if (WeatherFX)
		{
//			WeatherFX->SetNiagaraVariableVec3("Camera Location", CameraLocation);
			WeatherFX->SetNiagaraVariableFloat("Global Time Dilation", UGameplayStatics::GetGlobalTimeDilation(World));
		}
#if ENGINE_MAJOR_VERSION == 4
		if (bIsUsedWithSequencer)
		{
			UpdateSettings();
		}
#endif
	}
}

void ASkyCreator::SelectVolumetricCloudMaterial()
{
	if (bGroundContribution)
	{
		if (bRayMarchVolumeShadow)
		{
			switch (MultiScatteringApproximationOctaveCount)
			{
				case 0: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC1_RMVS1.M_SC_VolumetricClouds_MSAOC0_GC1_RMVS1'"));
					break;
				}
				case 1: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC1_RMVS1.M_SC_VolumetricClouds_MSAOC1_GC1_RMVS1'"));
					break;
				}
				case 2: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC2_GC1_RMVS1.M_SC_VolumetricClouds_MSAOC2_GC1_RMVS1'"));
					break;
				}
				default:
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC1_RMVS1.M_SC_VolumetricClouds_MSAOC0_GC1_RMVS1'"));
					break;
				}
			}
		}
		else 
		{
			switch (MultiScatteringApproximationOctaveCount)
			{
				case 0: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC1_RMVS0.M_SC_VolumetricClouds_MSAOC0_GC1_RMVS0'"));
					break;
				}
				case 1: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC1_RMVS0.M_SC_VolumetricClouds_MSAOC1_GC1_RMVS0'"));
					break;
				}
				case 2: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC2_GC1_RMVS0.M_SC_VolumetricClouds_MSAOC2_GC1_RMVS0'"));
					break;
				}
				default: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC1_RMVS0.M_SC_VolumetricClouds_MSAOC0_GC1_RMVS0'"));
					break;
				}
			}
		}
	}

	else if (!bGroundContribution)
	{
		if (bRayMarchVolumeShadow)
		{
			switch (MultiScatteringApproximationOctaveCount)
			{
				case 0: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC0_GC0_RMVS1'"));
					break;
				}
				case 1: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC1_GC0_RMVS1'"));
					break;
				}
				case 2: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC2_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC2_GC0_RMVS1'"));
					break;
				}
				default: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC0_RMVS1.M_SC_VolumetricClouds_MSAOC0_GC0_RMVS1'"));
					break;
				}
			}
		}
		else 
		{
			switch (MultiScatteringApproximationOctaveCount)
			{
				case 0: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC0_RMVS0.M_SC_VolumetricClouds_MSAOC0_GC0_RMVS0'"));
					break;
				}
				case 1: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC1_GC0_RMVS0.M_SC_VolumetricClouds_MSAOC1_GC0_RMVS0'"));
					break;
				}
				case 2: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC2_GC0_RMVS0.M_SC_VolumetricClouds_MSAOC2_GC0_RMVS0'"));
					break;
				}
				default: 
				{
					CurrentVolumetricCloudMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/SkyCreatorPlugin/Materials/VolumetricClouds/M_SC_VolumetricClouds_MSAOC0_GC0_RMVS0.M_SC_VolumetricClouds_MSAOC0_GC0_RMVS0'"));
					break;
				}
			}
		}
	}
}
/*
void ASkyCreator::RerunConstructionScript()
{
	RerunConstructionScripts();
}
*/
bool ASkyCreator::ShouldTickIfViewportsOnly() const
{
	if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor && bIsEditorTickEnabled)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASkyCreator::RealtimeVolumetricCloudWind(const float DeltaTime)
{
	if (bEnableWind)
	{
		// Increment cloud map UV offset by DeltaTime each tick
		CloudWindOffset = CloudWindOffset + ((DeltaTime * WeatherSettings.WindSettings.CloudWindSpeed) * FVector(UKismetMathLibrary::DegSin(WeatherSettings.WindSettings.CloudWindDirection), UKismetMathLibrary::DegCos(WeatherSettings.WindSettings.CloudWindDirection), 0));
		CloudWindSkewDirection = FVector(UKismetMathLibrary::DegSin(WeatherSettings.WindSettings.CloudWindDirection), UKismetMathLibrary::DegCos(WeatherSettings.WindSettings.CloudWindDirection), 0);
		//HighCloudWindOffset = HighCloudWindOffset + ((DeltaTime * WeatherSettings.WindSettings.HighCloudWindSpeed) * FVector(UKismetMathLibrary::DegSin(WeatherSettings.WindSettings.HighCloudWindDirection), UKismetMathLibrary::DegCos(WeatherSettings.WindSettings.HighCloudWindDirection), 0));
		CloudNoiseShapeWindOffset = CloudNoiseShapeWindOffset + ((DeltaTime * WeatherSettings.WindSettings.CloudNoiseShapeWindSpeedHorizontal) * FVector(UKismetMathLibrary::DegSin(WeatherSettings.WindSettings.CloudNoiseShapeWindDirection), UKismetMathLibrary::DegCos(WeatherSettings.WindSettings.CloudNoiseShapeWindDirection), 0));
		CloudNoiseDetailWindOffset.Z = CloudNoiseDetailWindOffset.Z + ((DeltaTime * (WeatherSettings.WindSettings.CloudNoiseDetailWindSpeedVertical * -1.0f)));

		// Keeps cloud map UV offset in 0-1 range
		CloudWindOffset = FVector(FMath::Fmod(CloudWindOffset.X, CloudMapScale * 100.0f), FMath::Fmod(CloudWindOffset.Y, CloudMapScale * 100.0f), 0);
		//HighCloudWindOffset = FVector(FMath::Fmod(HighCloudWindOffset.X, HighCloudMapScale * 100.0f), FMath::Fmod(HighCloudWindOffset.Y, HighCloudMapScale * 100.0f), 0);
		CloudNoiseShapeWindOffset = FVector(FMath::Fmod(CloudNoiseShapeWindOffset.X, NoiseShapeScale * 100.0f), FMath::Fmod(CloudNoiseShapeWindOffset.Y, NoiseShapeScale * 100.0f), 0.0f);
		CloudNoiseDetailWindOffset = FVector(0.0f, 0.0f, FMath::Fmod(CloudNoiseDetailWindOffset.Z, NoiseDetailScale * 100.0f));
	}
	else
	{

		CloudWindSkewDirection = FVector(0, 0, 0);
		CloudWindSkewForce = 0.0f;
		CloudWindOffset = FVector(0, 0, 0);
		CloudNoiseShapeWindOffset = FVector(0, 0, 0);
		CloudNoiseDetailWindOffset = FVector(0, 0, 0);
		//HighCloudWindOffset = FVector(0, 0, 0);
	}

	if (VolumetricCloudsMPC)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Cloud Map Wind Offset", FLinearColor(CloudWindOffset));
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Cloud Map Wind Skew Direction", FLinearColor(CloudWindSkewDirection));
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cloud Map Wind Skew Force", CloudWindSkewForce);
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Noise Shape Wind Offset", FLinearColor(CloudNoiseShapeWindOffset));
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Noise Detail Wind Offset", FLinearColor(CloudNoiseDetailWindOffset));
	}
}

void ASkyCreator::RipplesSolverPropagation()
{
//	UTextureRenderTarget2D* PropagationFrame0 = nullptr;
//	UTextureRenderTarget2D* PropagationFrame1 = nullptr;
//	UTextureRenderTarget2D* PropagationFrame2 = nullptr;

	if (!RainRipplesPropagationFrame0) RainRipplesPropagationFrame0 = UKismetRenderingLibrary::CreateRenderTarget2D(this, 256, 256, RTF_R16f);
	if (!RainRipplesPropagationFrame1) RainRipplesPropagationFrame1 = UKismetRenderingLibrary::CreateRenderTarget2D(this, 256, 256, RTF_R16f);
	if (!RainRipplesPropagationFrame2) RainRipplesPropagationFrame2 = UKismetRenderingLibrary::CreateRenderTarget2D(this, 256, 256, RTF_R16f);

	if (GetWorldTimerManager().TimerExists(RainRipplesTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(RainRipplesTimerHandle);
		if (RainRipplesPropagationFrame0 && RainRipplesPropagationFrame1 && RainRipplesPropagationFrame2)
		{
			RainRipplesSolverHeightState = (RainRipplesSolverHeightState + 1) % 3;

			if (RainRipplesForce)
			{
				UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RipplesSolverGetLastFrame(RainRipplesSolverHeightState, 1), RainRipplesForce);
			}

			if (RainRipplesPropagationMID)
			{
				RainRipplesPropagationMID->SetTextureParameterValue("Frame 1", RipplesSolverGetLastFrame(RainRipplesSolverHeightState, 1));
				RainRipplesPropagationMID->SetTextureParameterValue("Frame 2", RipplesSolverGetLastFrame(RainRipplesSolverHeightState, 2));

				UCanvas* Canvas = nullptr;
				FVector2D CanvasSize = FVector2D(0, 0);
				FDrawToRenderTargetContext Context;
				UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RipplesSolverGetFrame(RainRipplesSolverHeightState), Canvas, CanvasSize, Context);
				Canvas->K2_DrawMaterial(RainRipplesPropagationMID, FVector2D(0), CanvasSize, FVector2D(0), FVector2D(1, 1));
				UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
			}

			if (RainRipplesNormalMID && RainRipplesNormalRT)
			{
				RainRipplesNormalMID->SetTextureParameterValue("Current Frame", RipplesSolverGetFrame(RainRipplesSolverHeightState));
				UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RainRipplesNormalRT, RainRipplesNormalMID);
			}
		}

		if (CommonMPC)
		{
			UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Solver Delta", RainRipplesSolverDelta);
		}
	}
}

void ASkyCreator::RipplesSolverClearRT()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RainRipplesPropagationFrame0);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RainRipplesPropagationFrame1);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RainRipplesPropagationFrame2);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RainRipplesNormalRT);
}

UTextureRenderTarget2D* ASkyCreator::RipplesSolverGetFrame(const int32 Index) const
{
	UTextureRenderTarget2D* Frame = nullptr;

	switch (Index)
	{
		case 0: 
		{
			Frame = RainRipplesPropagationFrame0;
			break;
		}
		case 1: 
		{
			Frame = RainRipplesPropagationFrame1;
			break;
		}
		case 2: 
		{
			Frame = RainRipplesPropagationFrame2;
			break;
		}
		default: 
		{
			Frame = RainRipplesPropagationFrame0;
			break;
		}
	}

	return Frame;
}

UTextureRenderTarget2D* ASkyCreator::RipplesSolverGetLastFrame(const int32 Index, const int32 NumFramesOld) const
{
	return RipplesSolverGetFrame((Index - NumFramesOld + 3) % 3);
}

FVector ASkyCreator::GetCurrentCameraPosition() const
{
	FVector Result;
	const UWorld* World = GetWorld();
	check(World != nullptr);

	if (World->HasBegunPlay() && HasAuthority() == false /*!UKismetSystemLibrary::IsDedicatedServer(this) && !UKismetSystemLibrary::IsServer(this)*/)
	{
		const APlayerCameraManager* CameraManager = World->GetFirstPlayerController()->PlayerCameraManager;
		if (CameraManager)
		{
			Result = CameraManager->GetCameraLocation();
		}
	}
	else 
	{
		auto ViewLocations = World->ViewLocationsRenderedLastFrame;
		if (ViewLocations.Num() != 0)
		{
			Result = ViewLocations[0];
		}
	}

	return Result;
}

FVector ASkyCreator::GetLastLightningPosition()
{
	return LastLightningPosition;
}

float ASkyCreator::GetCloudDensityAtPosition(FVector Position)
{
	float Density = 0.0f;

	if (VolumetricCloudsMPC && VolumetricCloudDensitySampleMID && VolumetricCloudDensitySampleRT)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(this, VolumetricCloudDensitySampleRT, FLinearColor(0, 0, 0, 1));
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Cloud Density Sample Position", FLinearColor(Position));
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, VolumetricCloudDensitySampleRT, VolumetricCloudDensitySampleMID);
		const FLinearColor CurrentColor = UKismetRenderingLibrary::ReadRenderTargetRawPixel(this, VolumetricCloudDensitySampleRT, 0, 0); // this is slow
		Density = CurrentColor.R;
	}

	return Density;
}

bool ASkyCreator::FindLightningPosition(FVector Position, FVector& OutPosition)
{
	bool Result = false;
	
	FVector Position2D = FVector::ZeroVector;
	FVector ResultPosition = FVector::ZeroVector;

	if (bSampleCloudDensity)
	{
		for (int i = 0; i < LightningMaxSamples; i++)
		{
			Position2D = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(LightningSpawnInnerRadius), USkyCreatorFunctionLibrary::KilometersToCentimeters(LightningSpawnOuterRadius)) + Position;
			ResultPosition = FVector(Position2D.X, Position2D.Y, UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(LayerBottomAltitude), USkyCreatorFunctionLibrary::KilometersToCentimeters(LayerBottomAltitude + LayerHeight * 0.3f)));
			const float CurrentCloudDensity = GetCloudDensityAtPosition(ResultPosition);
//			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("Cloud Sample Position: %s"), *ResultPosition.ToString()));
			const float DensityThreshold = 0.01f;
			
			if (CurrentCloudDensity >= DensityThreshold)
			{
				Result = true;
				//UKismetSystemLibrary::DrawDebugPoint(WorldContextObject, ResultPosition, 100.f, FLinearColor::Green, 1.0f);
				OutPosition = ResultPosition;
				break;
			}
			else
			{
				Result = false;
				//UKismetSystemLibrary::DrawDebugPoint(WorldContextObject, ResultPosition, 50.f, FLinearColor::Red, 1.0f);
			}
		}
	}
	else
	{
		Position2D = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(LightningSpawnInnerRadius), USkyCreatorFunctionLibrary::KilometersToCentimeters(LightningSpawnOuterRadius)) + Position;
		ResultPosition = FVector(Position2D.X, Position2D.Y, UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(LayerBottomAltitude), USkyCreatorFunctionLibrary::KilometersToCentimeters(LayerBottomAltitude + LayerHeight * 0.3f)));
		Result = true;
		OutPosition = ResultPosition;
	}

	return Result;
}

void ASkyCreator::CheckOcclusion()
{
	// Snapped Camera Position to avoid pixel bleeding
	CameraLocationSnapped = UKismetMathLibrary::Vector_SnappedToGrid(CameraLocation + FVector(0, 0, OcclusionCaptureHeight), OcclusionCaptureStepSize);

	if (bOcclusionCaptureRealtimeUpdate)
	{
		// Realtime Update
		CaptureOcclusion(CameraLocationSnapped);
	}
	else
	{
		// Capture based on max distace with last position
		if (UKismetMathLibrary::Vector_Distance(CameraLocationSnapped, OcclusionCurrentLocation) >= OcclusionCaptureStepDistance)
		{
			CaptureOcclusion(CameraLocationSnapped);
//				UE_LOG(LogTemp, Warning, TEXT("Capture happened at CameraLocation = %s"), *CameraLocation.ToString());
		}
	}

	if (WeatherFX)
	{
		WeatherFX->SetNiagaraVariableVec3("Occlusion Current Location", OcclusionCurrentLocation);
/*
#if ENGINE_MAJOR_VERSION == 4
		WeatherFX->SetNiagaraVariableVec3("Occlusion Current Location", OcclusionCurrentLocation);
#endif
#if ENGINE_MAJOR_VERSION == 5
		WeatherFX->SetNiagaraVariablePosition("Occlusion Current Position", OcclusionCurrentLocation);
#endif
*/
	}
}

void ASkyCreator::CaptureOcclusion(FVector CaptureLocation)
{
	OcclusionCurrentLocation = CaptureLocation;
	OcclusionCapture->SetWorldLocation(OcclusionCurrentLocation);
	OcclusionCapture->SetWorldRotation(FRotator(-90, 270, 0));
	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Occlusion Capture Position", FLinearColor(OcclusionCapture->GetComponentLocation() - FVector(OcclusionCaptureWidth / 2, OcclusionCaptureWidth / 2, 0)));
	}
	OcclusionCapture->CaptureSceneDeferred();
}

void ASkyCreator::WriteLightningParameters(FVector InLightningPosition, float InLightningRadius, FLinearColor InLightningColor)
{
	if (LightningsParametersRT)
	{

		if (LightningCurrentIndex != LightningsParametersRT->SizeX - 1)
		{
			LightningCurrentIndex++;
		}
		else
		{
			LightningCurrentIndex = 0;
		}

		LightningParameters[LightningCurrentIndex] = { FLinearColor(InLightningPosition.X, InLightningPosition.Y, InLightningPosition.Z, InLightningRadius) , InLightningColor };

		UCanvas* Canvas = nullptr;
		FVector2D CanvasSize = FVector2D(0, 0);
		FDrawToRenderTargetContext Context;
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, LightningsParametersRT, Canvas, CanvasSize, Context);

		// Write Lightning Position
		Canvas->K2_DrawTexture(nullptr, FVector2D(UKismetMathLibrary::Conv_IntToFloat(LightningCurrentIndex), 0), FVector2D(1, 1), FVector2D(0, 0), FVector2D(1, 1), LightningParameters[LightningCurrentIndex].Position, BLEND_Opaque);
		// Write Lightning Color
		Canvas->K2_DrawTexture(nullptr, FVector2D(UKismetMathLibrary::Conv_IntToFloat(LightningCurrentIndex), 1), FVector2D(1, 1), FVector2D(0, 0), FVector2D(1, 1), LightningParameters[LightningCurrentIndex].Color, BLEND_Opaque);

		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
	}
}

void ASkyCreator::LightningFlashFade()
{
	if (GetWorldTimerManager().TimerExists(LightningFlashFadeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LightningFlashFadeTimerHandle);
		if (LightningsParametersRT)
		{
			UCanvas* Canvas = nullptr;
			FVector2D CanvasSize = FVector2D(0, 0);
			FDrawToRenderTargetContext Context;
			UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, LightningsParametersRT, Canvas, CanvasSize, Context);
			// Write Fade
			Canvas->K2_DrawTexture(nullptr, FVector2D(0, 1), FVector2D(32, 1), FVector2D(0, 0), FVector2D(1, 1), FLinearColor(0, 0, 0, LightningFlashFadeSpeed), BLEND_Translucent);
			UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
		}
	}
}

void ASkyCreator::SpawnLightningStrike(FVector LightningPosition)
{
	const FVector LightningBoltEndPosition = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(FVector(0, 0, -1), LightningRandomDegreeInConeMax) * UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(WeatherSettings.WeatherFXSettings.LightningBoltLengthMin), USkyCreatorFunctionLibrary::KilometersToCentimeters(WeatherSettings.WeatherFXSettings.LightningBoltLengthMax));

	const FLinearColor CurrentLightningColor = FLinearColor(
		UKismetMathLibrary::RandomFloatInRange(WeatherSettings.WeatherFXSettings.LightningColorMin.R, WeatherSettings.WeatherFXSettings.LightningColorMax.R),
		UKismetMathLibrary::RandomFloatInRange(WeatherSettings.WeatherFXSettings.LightningColorMin.G, WeatherSettings.WeatherFXSettings.LightningColorMax.G),
		UKismetMathLibrary::RandomFloatInRange(WeatherSettings.WeatherFXSettings.LightningColorMin.B, WeatherSettings.WeatherFXSettings.LightningColorMax.B),
		UKismetMathLibrary::RandomFloatInRange(WeatherSettings.WeatherFXSettings.LightningColorMin.A, WeatherSettings.WeatherFXSettings.LightningColorMax.A));

	const float CurrentLightningBoltSpawnChance = UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f);

	//UE_LOG(LogTemp, Warning, TEXT("CurrentLightningSpawnChance: %f"), CurrentLightningSpawnChance);

	if (CurrentLightningBoltSpawnChance <= WeatherSettings.WeatherFXSettings.LightningBoltSpawnChance)
	{
		// Find and Spawn Lightning System
		UNiagaraSystem* LightningStrikeSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/SkyCreatorPlugin/Effects/SC_WeatherFX_LightningStrike_System.SC_WeatherFX_LightningStrike_System'"));

		if (LightningStrikeSystem)
		{
			UNiagaraComponent* LightningStrikeComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, LightningStrikeSystem, LightningPosition, FRotator(0, 0, 0), FVector(1, 1, 1), EAttachLocation::Type::KeepRelativeOffset, true);

			if (LightningStrikeComponent)
			{
				LightningStrikeComponent->SetNiagaraVariableVec3("LightningBoltEndPosition", LightningBoltEndPosition);
				LightningStrikeComponent->SetNiagaraVariableLinearColor("LightningColor", CurrentLightningColor * LightningBoltEmissiveScale);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningLifetime", WeatherSettings.WeatherFXSettings.LightningLifetime);

				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltWidthMin", WeatherSettings.WeatherFXSettings.LightningBoltWidthMin);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltWidthMax", WeatherSettings.WeatherFXSettings.LightningBoltWidthMax);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltCurveFrequency", WeatherSettings.WeatherFXSettings.LightningBoltCurveFrequency);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltCurveStrengthMin", WeatherSettings.WeatherFXSettings.LightningBoltCurveStrengthMin);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltCurveStrengthMax", WeatherSettings.WeatherFXSettings.LightningBoltCurveStrengthMax);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBoltJitterStrength", WeatherSettings.WeatherFXSettings.LightningBoltJitterStrength);

				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBranchSpawnChance", WeatherSettings.WeatherFXSettings.LightningBranchSpawnChance);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBranchScaleMin", WeatherSettings.WeatherFXSettings.LightningBranchScaleMin);
				LightningStrikeComponent->SetNiagaraVariableFloat("LightningBranchScaleMax", WeatherSettings.WeatherFXSettings.LightningBranchScaleMax);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to load Lightning System asset!"));
		}
	}

	const float LightningFlashRadius = LightningBoltEndPosition.Size();

	WriteLightningParameters(LightningPosition, LightningFlashRadius * LightningFlashRadiusScale, CurrentLightningColor * LightningFlashEmissiveScale);

	LastLightningPosition = LightningPosition;

	OnLightningStrike.Broadcast(LightningPosition);
}

void ASkyCreator::MakeLightningStrike()
{
	if (GetWorldTimerManager().TimerExists(LightningIntervalTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LightningIntervalTimerHandle);
		//GEngine->AddOnScreenDebugMessage(-1, 0.25f, FColor::Red, FString::Printf(TEXT("Lightning Strike!!!")));
		//bool FoundLightningPosition = USkyCreatorFunctionLibrary::FindLightningPosition(this, bSampleCloudDensity, LightningMaxSamples, VolumetricCloudsMPC, VolumetricCloudDensitySampleMID, VolumetricCloudDensitySampleRT, GetCurrentCameraPosition(), LightningSpawnInnerRadius, LightningSpawnOuterRadius, LayerBottomAltitude, LayerBottomAltitude + LayerHeight * 0.3f, 0.01f, LightningPosition);
		FVector LightningPosition;
		const bool FoundLightningPosition = FindLightningPosition(GetCurrentCameraPosition(), LightningPosition);
		if (FoundLightningPosition)
		{
			SpawnLightningStrike(LightningPosition);
		}
	}
}

void ASkyCreator::SetComponentsSettings()
{
	if (SkyAtmosphere)
	{
		SkyAtmosphere->TransformMode = TransformMode;
		SkyAtmosphere->BottomRadius = PlanetRadius;
		SkyAtmosphere->AtmosphereHeight = AtmosphereHeight;
		SkyAtmosphere->AerialPespectiveViewDistanceScale = AerialPespectiveViewDistanceScale;
		SkyAtmosphere->TraceSampleCountScale = TraceSampleCountScale;

		static const auto VarAerialPerspectiveLUTWidth = IConsoleManager::Get().FindTConsoleVariableDataFloat(TEXT("r.SkyAtmosphere.AerialPerspectiveLUT.Width"));
		const float VarAerialPerspectiveLUTWidthValue = VarAerialPerspectiveLUTWidth->GetValueOnAnyThread();

		if (VarAerialPerspectiveLUTWidthValue != GodRaysResolution)
		{
			USkyCreatorFunctionLibrary::SetConsoleCommandFloat(this, "r.SkyAtmosphere.AerialPerspectiveLUT.Width ", GodRaysResolution);
		}
	}

	if (VolumetricCloud)
	{
		VolumetricCloud->SetLayerBottomAltitude(LayerBottomAltitude);
		VolumetricCloud->SetLayerHeight(LayerHeight);
		VolumetricCloud->SetTracingStartMaxDistance(TracingStartMaxDistance);
		VolumetricCloud->SetTracingMaxDistance(TracingMaxDistance);
		VolumetricCloud->SetShadowTracingDistance(ShadowTracingDistance);
		VolumetricCloud->SetbUsePerSampleAtmosphericLightTransmittance(bPerSampleAtmosphericLightTransmittance);

		static const auto VarVolumetricRenderTarget = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.VolumetricRenderTarget"));
		const bool bVarVolumetricRenderTarget = UKismetMathLibrary::Conv_IntToBool(VarVolumetricRenderTarget->GetValueOnAnyThread());

		if (bVarVolumetricRenderTarget != !bCinematicQuality)
		{
			USkyCreatorFunctionLibrary::SetConsoleCommandInt(this, "r.VolumetricRenderTarget ", !bCinematicQuality);
		}

		static const auto VarVolumetricRenderTargetMode = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.VolumetricRenderTarget.Mode"));
		const int32 VarVolumetricRenderTargetModeValue = VarVolumetricRenderTargetMode->GetValueOnAnyThread();

		if (VarVolumetricRenderTargetModeValue != RenderMode)
		{
			USkyCreatorFunctionLibrary::SetConsoleCommandInt(this, "r.VolumetricRenderTarget.Mode ", RenderMode);
		}

		static const auto VarHighQualityAerialPerspective = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.VolumetricCloud.HighQualityAerialPerspective"));
		const bool bVarHighQualityAerialPerspectiveValue = UKismetMathLibrary::Conv_IntToBool(VarHighQualityAerialPerspective->GetValueOnAnyThread());

		if (bVarHighQualityAerialPerspectiveValue != bHighQualityAerialPerspective)
		{
			USkyCreatorFunctionLibrary::SetConsoleCommandInt(this, "r.VolumetricCloud.HighQualityAerialPerspective ", bHighQualityAerialPerspective);
		}

		VolumetricCloud->SetViewSampleCountScale(ViewSampleCountScale);
		VolumetricCloud->SetShadowViewSampleCountScale(ShadowViewSampleCountScale);

#if ENGINE_MAJOR_VERSION == 4
		VolumetricCloud->SetReflectionSampleCountScale(ReflectionSampleCountScale);
		VolumetricCloud->SetShadowReflectionSampleCountScale(ShadowReflectionSampleCountScale);
#endif

#if ENGINE_MAJOR_VERSION == 5
		VolumetricCloud->SetReflectionViewSampleCountScale(ReflectionSampleCountScale);
		VolumetricCloud->SetShadowReflectionViewSampleCountScale(ShadowReflectionSampleCountScale);
#endif

		SelectVolumetricCloudMaterial();

		USkyCreatorFunctionLibrary::CreateAndAssignMID(this, CurrentVolumetricCloudMaterial, VolumetricCloudMID);

		if (VolumetricCloudMID)
		{
			VolumetricCloud->SetMaterial(VolumetricCloudMID);

			UTexture* NoiseShapeTexture = nullptr;
			UTexture* NoiseDetailTexture = nullptr;

			switch (NoiseShapeResolution)
			{
				case EVolumetricCloudNoiseShapeResolution::VolumetricCloudNoiseShapeResolution_64:
				{
					NoiseShapeTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Shape_64.VT_SC_CloudNoise_Shape_64'"));
					break;
				}
				case EVolumetricCloudNoiseShapeResolution::VolumetricCloudNoiseShapeResolution_128:
				{
					NoiseShapeTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Shape_128.VT_SC_CloudNoise_Shape_128'"));
					break;
				}
				case EVolumetricCloudNoiseShapeResolution::VolumetricCloudNoiseShapeResolution_256:
				{
					NoiseShapeTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Shape_256.VT_SC_CloudNoise_Shape_256'"));
					break;
				}
				default:
				{
					NoiseShapeTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Shape_128.VT_SC_CloudNoise_Shape_64'"));
					break;
				}
			}

			switch (NoiseDetailResolution)
			{
				case EVolumetricCloudNoiseDetailResolution::VolumetricCloudNoiseDetailResolution_32:
				{
					NoiseDetailTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Detail_32.VT_SC_CloudNoise_Detail_32'"));
					break;
				}
				case EVolumetricCloudNoiseDetailResolution::VolumetricCloudNoiseDetailResolution_64:
				{
					NoiseDetailTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Detail_64.VT_SC_CloudNoise_Detail_64'"));
					break;
				}
				default:
				{
					NoiseDetailTexture = LoadObject<UTexture>(nullptr, TEXT("Texture'/SkyCreatorPlugin/Textures/VolumetricClouds/VT_SC_CloudNoise_Detail_32.VT_SC_CloudNoise_Detail_32'"));
					break;
				}
			}

			if (NoiseShapeTexture)
			{
				VolumetricCloudMID->SetTextureParameterValue("NoiseShape_Texture", NoiseShapeTexture);
			}

			if (NoiseDetailTexture)
			{
				VolumetricCloudMID->SetTextureParameterValue("NoiseDetail_Texture", NoiseDetailTexture);
			}
		}

		if (!VolumetricCloudDensitySampleRT)
		{
			VolumetricCloudDensitySampleRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1, 1, RTF_RGBA16f);
		}

		USkyCreatorFunctionLibrary::CreateAndAssignMID(this, VolumetricCloudDensitySampleMaterial, VolumetricCloudDensitySampleMID);
	}

//	CreateAndAssignMID(CloudMapMaterial, CloudMapMID);

	if (SkyLight)
	{
		SkyLight->SetMobility(SkyLightMobility);
		SkyLight->bRealTimeCapture = bRealTimeCapture;

		static const auto VarTimeSlicedRealTimeCapture = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.SkyLight.RealTimeReflectionCapture.TimeSlice"));
		const bool bVarTimeSlicedRealTimeCapture = UKismetMathLibrary::Conv_IntToBool(VarTimeSlicedRealTimeCapture->GetValueOnAnyThread());

		if (bVarTimeSlicedRealTimeCapture != bSkyLightCaptureTimeSlice)
		{
			USkyCreatorFunctionLibrary::SetConsoleCommandInt(this, "r.SkyLight.RealTimeReflectionCapture.TimeSlice ", bSkyLightCaptureTimeSlice);
		}

		SkyLight->bLowerHemisphereIsBlack = bLowerHemisphereIsSolidColor;
		//	SkyLight->LowerHemisphereColor = LowerHemisphereColor;
		SkyLight->bCloudAmbientOcclusion = bCloudAmbientOcclusion;
		SkyLight->CloudAmbientOcclusionExtent = CloudAmbientOcclusionExtent;
		SkyLight->CloudAmbientOcclusionMapResolutionScale = CloudAmbientOcclusionMapResolutionScale;
		SkyLight->CloudAmbientOcclusionApertureScale = CloudAmbientOcclusionApertureScale;
		SkyLight->SetCaptureIsDirty();
	}

	if (SunLight)
	{
		SunLight->SetMobility(SunLightMobility);
		if (bSunConstantIntensity)
		{
			SunLight->SetIntensity(SunIntensity);
		}
		SunLight->LightSourceAngle = SunDiskSize;
		SunLight->bUseTemperature = bSunUseTemperature;
		SunLight->AtmosphereSunDiskColorScale = SunAtmosphereDiskColorScale;
		SunLight->bPerPixelAtmosphereTransmittance = bSunPerPixelAtmosphereTransmittance;
		SunLight->CloudShadowExtent = SunCloudShadowExtent;
		SunLight->CloudShadowMapResolutionScale = SunCloudShadowMapResolutionScale;
		SunLight->CloudShadowRaySampleCountScale = SunCloudShadowRaySampleCountScale;
		SunLight->MarkRenderStateDirty();
//		SunLight->DisabledBrightness = SunBrightness;
	}

	if (MoonLight)
	{
		MoonLight->SetMobility(MoonLightMobility);
		if (bMoonConstantIntensity)
		{
			MoonLight->SetIntensity(MoonIntensity);
		}
		MoonLight->LightSourceAngle = MoonDiskSize;
		MoonLight->bUseTemperature = bMoonUseTemperature;
		MoonLight->AtmosphereSunDiskColorScale = MoonAtmosphereDiskColorScale;
		MoonLight->bPerPixelAtmosphereTransmittance = bMoonPerPixelAtmosphereTransmittance;
		MoonLight->CloudShadowExtent = MoonCloudShadowExtent;
		MoonLight->CloudShadowMapResolutionScale = MoonCloudShadowMapResolutionScale;
		MoonLight->CloudShadowRaySampleCountScale = MoonCloudShadowRaySampleCountScale;
		MoonLight->MarkRenderStateDirty();
//		MoonLight->DisabledBrightness = MoonBrightness;
	}

	if (ExponentialHeightFog)
	{
		ExponentialHeightFog->SetMobility(ExponentialHeightFogMobility);
		ExponentialHeightFog->SetVolumetricFog(bVolumetricFog);
		ExponentialHeightFog->SetRelativeLocation(FVector(0, 0, FogHeightOffset));
		ExponentialHeightFog->SecondFogData.FogHeightOffset = SecondFogHeightOffset;

		if (bEnableExponentialHeightFog)
		{
			ExponentialHeightFog->SetVisibility(true);
			ExponentialHeightFog->SetHiddenInGame(false);
		}
		else
		{
			ExponentialHeightFog->SetVisibility(false);
			ExponentialHeightFog->SetHiddenInGame(true);
		}
	}

	if (SkySphere && SkySphereMaterial)
	{
		if (!SkySphereMID || (SkySphereMID->Parent != SkySphereMaterial))
		{
			SkySphereMID = UMaterialInstanceDynamic::Create(SkySphereMaterial, this);
		}

		SkySphere->SetMaterial(0, SkySphereMID);
		SkySphere->SetRelativeScale3D(FVector(SkySphereRadius, SkySphereRadius, SkySphereRadius));
		SkySphereMID->SetTextureParameterValue("StarMap_Texture", StarMapTexture);
	}

	if (OcclusionCapture)
	{
		if (OcclusionRenderTarget)
		{
			OcclusionCapture->TextureTarget = OcclusionRenderTarget;
		}
		OcclusionCapture->OrthoWidth = OcclusionCaptureWidth;

		OcclusionCaptureStepSize = OcclusionCaptureWidth / FMath::Max(OcclusionRenderTarget->SizeX, OcclusionRenderTarget->SizeY);

		CheckOcclusion();
	}

	SetMPCSettings();

	SetWeatherFXStaticSettings();

	SetWeatherMaterialFXStaticSettings();

	USkyCreatorFunctionLibrary::CreateAndAssignMID(this, RainRipplesPropagation, RainRipplesPropagationMID);
	USkyCreatorFunctionLibrary::CreateAndAssignMID(this, RainRipplesNormal, RainRipplesNormalMID);

	LightningFlashFadeDelta = 1.f / LightningFlashFadeUpdateRate;
	RainRipplesSolverDelta = 1.f / RainRipplesUpdateRate;

	if (LightningsParametersRT)
	{
		const int32 Size = LightningsParametersRT->SizeX;
		LightningParameters.Init({ FLinearColor(0, 0, 0, 1000), FLinearColor(0, 0, 0, 1000) }, Size);
	}

	if (PostProcess)
	{
		PostProcess->bEnabled = bUseExposureSettings;

		if (bUseExposureSettings)
		{
			PostProcess->Priority = PostProcessPriority;
			PostProcess->Settings.AutoExposureMethod = ExposureMethod;
			PostProcess->Settings.AutoExposureLowPercent = ExposureLowPercent;
			PostProcess->Settings.AutoExposureHighPercent = ExposureHighPercent;
			PostProcess->Settings.AutoExposureSpeedUp = ExposureSpeedUp;
			PostProcess->Settings.AutoExposureSpeedDown = ExposureSpeedDown;
			//		Exposure->Settings.AutoExposureBias = ExposureSettings.Bias;
			PostProcess->Settings.AutoExposureBiasCurve = ExposureBiasCurve;
			PostProcess->Settings.AutoExposureMeterMask = ExposureMeterMask;
			//		Exposure->Settings.AutoExposureCalibrationConstant_DEPRECATED = ExposureSettings.CalibrationConstant;
			//		Exposure->Settings.AutoExposureApplyPhysicalCameraExposure = ExposureSettings.ApplyPhysicalCameraExposure;

			if (bExtendDefaultLuminanceRange)
			{
				PostProcess->Settings.AutoExposureMinBrightness = ExposureMinEV100;
				PostProcess->Settings.AutoExposureMaxBrightness = ExposureMaxEV100;
				PostProcess->Settings.HistogramLogMin = ExposureHistogramMinEV100;
				PostProcess->Settings.HistogramLogMax = ExposureHistogramMaxEV100;
			}
			else
			{
				PostProcess->Settings.AutoExposureMinBrightness = ExposureMinBrightness;
				PostProcess->Settings.AutoExposureMaxBrightness = ExposureMaxBrightness;
				PostProcess->Settings.HistogramLogMin = ExposureHistogramLogMin;
				PostProcess->Settings.HistogramLogMax = ExposureHistogramLogMax;
			}
		}
	}
}

void ASkyCreator::SetMPCSettings()
{
	switch (TransformMode)
	{
		case ESkyAtmosphereTransformMode::PlanetTopAtAbsoluteWorldOrigin:
		{
			PlanetTopPosition = FVector(0.0f, 0.0f, 0.0f);
			break;
		}
		case ESkyAtmosphereTransformMode::PlanetTopAtComponentTransform:
		{
			PlanetTopPosition = GetActorLocation() - SkyAtmosphere->GetRelativeLocation();
			break;
		}
		case ESkyAtmosphereTransformMode::PlanetCenterAtComponentTransform:
		{
			PlanetTopPosition = GetActorLocation() - SkyAtmosphere->GetRelativeLocation() + FVector(0.0f, 0.0f, USkyCreatorFunctionLibrary::KilometersToCentimeters(PlanetRadius));
			break;
		}
		default:
		{
			PlanetTopPosition = FVector(0.0f, 0.0f, 0.0f);
			break;
		}
	}

	LayerBottomAltitudePosition = PlanetTopPosition + FVector(0.0f, 0.0f, USkyCreatorFunctionLibrary::KilometersToCentimeters(VolumetricCloud->LayerBottomAltitude));
	LayerTopAltitudePosition = PlanetTopPosition + FVector(0.0f, 0.0f, USkyCreatorFunctionLibrary::KilometersToCentimeters(VolumetricCloud->LayerBottomAltitude + VolumetricCloud->LayerHeight));

	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Capture Width", OcclusionCaptureWidth);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Bias", OcclusionBias);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Blur Samples", UKismetMathLibrary::Conv_IntToFloat(OcclusionBlurSamples));
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Blur Distance", OcclusionBlurDistance / OcclusionCaptureWidth); // Blur Distance in world units
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Mask Radius", (OcclusionCaptureWidth / 2) - OcclusionCaptureStepDistance);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Mask Fade Hardness", OcclusionMaskFadeHardness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Slope Angle", WetnessSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Slope Smoothness", WetnessSlopeSmoothness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Scale", PuddlesMaskScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Roughness", PuddlesRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Slope Angle", PuddlesSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Slope Smoothness", PuddlesSlopeSmoothness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Scale", RainRipplesScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Max Density", RainRipplesMaxDensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wind Ripples Scale", WindRipplesScale);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Mask Scale", SnowMaskScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Scale", SnowScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Roughness", SnowRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Sparkles Scale", SnowSparklesScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Sparkles Roughness", SnowSparklesRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Slope Angle", SnowSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Slope Smoothness", SnowSlopeSmoothness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Sky Atmosphere Planet Top", PlanetTopPosition.Z);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Volumetric Cloud Bottom Altitude", LayerBottomAltitudePosition.Z);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Volumetric Cloud Top Altitude", LayerTopAltitudePosition.Z);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Angular Diameter", MoonDiskSize);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Rotation", MoonRotation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Phase", MoonPhase);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Star Map Rotation", FLinearColor(GetStarMapRotation().Vector()));
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Star Map Additional Rotation", FLinearColor(StarMapAdditionalRotation));

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Night Intensity Transition Start Sun Angle", NightIntensityTransitionStartSunAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Night Intensity Transition End Sun Angle", NightIntensityTransitionEndSunAngle);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Contrast", BackgroundCloudsContrast);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Reflection Scale", BackgroundCloudsReflectionScale);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Weather FX Cutoff World Height", WeatherFXCutoffWorldHeight);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Weather FX Cutoff Softness", WeatherFXCutoffSoftness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Material FX Cutoff World Height", MaterialFXCutoffWorldHeight);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Material FX Cutoff Softness", MaterialFXCutoffSoftness);

		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Sun Light Direction", FLinearColor(SunLight->GetComponentRotation().Vector() * -1.0f));
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Moon Light Direction", FLinearColor(MoonLight->GetComponentRotation().Vector() * -1.0f));
	}

	if (VolumetricCloudsMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cloud Map Scale", CloudMapScale);
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Cloud Map Offset", FLinearColor(CloudMapOffset.X, CloudMapOffset.Y, 0));
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Coverage Variation Map Scale", CoverageVariationMapScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Lightning Flash Reflection Scale", LightningFlashEmissiveReflectionScale);
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Noise Shape Wind Offset", FLinearColor(CloudNoiseShapeWindOffset));

		VolumetricCloudMID->SetScalarParameterValue("NoiseShape_Scale", NoiseShapeScale);
		VolumetricCloudMID->SetScalarParameterValue("NoiseDetail_Scale", NoiseDetailScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Shape Scale", NoiseShapeScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Detail Scale", NoiseDetailScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Turbulence Scale", TurbulenceScale);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Sky Atmosphere Planet Top", PlanetTopPosition.Z);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Volumetric Cloud Bottom Altitude", LayerBottomAltitudePosition.Z);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Volumetric Cloud Top Altitude", LayerTopAltitudePosition.Z);
	}
}

void ASkyCreator::SetWeatherFXStaticSettings()
{
	if (WeatherFX)
	{
		// A known bug in Niagara System containing GPU Emitters is that you can't safely load the asset during actor construction.
		// This crash will be expected in a packaged build: https://forums.unrealengine.com/t/niagara-shader-module-delegate-never-set/463402
		// This is a workaround.
		if (WeatherFX->GetAsset() == nullptr)
		{
			WeatherFX->SetAsset(LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/SkyCreatorPlugin/Effects/SC_WeatherFX_System.SC_WeatherFX_System'")));
//			UE_LOG(LogTemp, Warning, TEXT("Trying to get an asset!"));
		}

		if (bEnableGPUPrecipitation && bEnableOcclusionCapture)
		{
			WeatherFX->SetNiagaraVariableBool("Precipitation GPU", true);
		}
		else
		{
			WeatherFX->SetNiagaraVariableBool("Precipitation GPU", false);
		}

		WeatherFX->SetNiagaraVariableFloat("Precipitation Spawn Radius", PrecipitationSpawnRadius);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Spawn Radius GPU", PrecipitationSpawnRadiusGPU);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Depth Fade Distance", PrecipitationDepthFadeDistance);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Camera Fade Distance", PrecipitationCameraFadeDistance);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Camera Fade Offset", PrecipitationCameraFadeOffset);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Max View Distance", PrecipitationMaxViewDistance);
		WeatherFX->SetNiagaraVariableFloat("Precipitation Vertical Check Distance", PrecipitationVerticalCheckDistance);
		WeatherFX->SetNiagaraVariableInt("Precipitation Collision Channel", UKismetMathLibrary::Conv_ByteToInt(PrecipitationCollisionChannel));
		WeatherFX->SetNiagaraVariableFloat("Cloud Bottom Altitude", LayerBottomAltitudePosition.Z);

		WeatherFX->SetNiagaraVariableFloat("Rain Distance Scale Factor", RainDistanceScaleFactor);
		WeatherFX->SetNiagaraVariableFloat("Rain Camera Velocity Alignment Scale", RainCameraMotionAlignmentScale);
		WeatherFX->SetNiagaraVariableFloat("Rain Velocity Stretching Scale", RainVelocityElongationScale);
//		WeatherFX->SetNiagaraVariableFloat("Rain IOR", RainIndexOfRefraction);
		WeatherFX->SetNiagaraVariableFloat("Rain Mask Hardness", RainMaskHardness);
//		WeatherFX->SetNiagaraVariableFloat("Rain Splash IOR", RainSplashIndexOfRefraction);

		WeatherFX->SetNiagaraVariableFloat("Snow Distance Scale Factor", SnowDistanceScaleFactor);
		WeatherFX->SetNiagaraVariableFloat("Snow Camera Velocity Alignment Scale", SnowCameraMotionAlignmentScale);
		WeatherFX->SetNiagaraVariableFloat("Snow Velocity Stretching Scale", SnowVelocityElongationScale);
		WeatherFX->SetNiagaraVariableFloat("Snow Mask Hardness", SnowMaskHardness);

		WeatherFX->SetNiagaraVariableFloat("Occlusion Width", OcclusionCaptureWidth);
		WeatherFX->SetNiagaraVariableObject("Occlusion Texture", OcclusionRenderTarget);

		WeatherFX->SetNiagaraVariableFloat("Rainbow Distance", RainbowDistance);
		WeatherFX->SetNiagaraVariableFloat("Rainbow Depth Fade Distance", RainbowDepthFadeDistance);

		WeatherFX->SetNiagaraVariableFloat("Cutoff World Height", WeatherFXCutoffWorldHeight);
	}
}

void ASkyCreator::SetWeatherMaterialFXStaticSettings()
{
	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Capture Width", OcclusionCaptureWidth);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Bias", OcclusionBias);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Blur Samples", UKismetMathLibrary::Conv_IntToFloat(OcclusionBlurSamples));
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Blur Distance", OcclusionBlurDistance/OcclusionCaptureWidth); // Blur Distance in world units
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Mask Radius", (OcclusionCaptureWidth/2) - OcclusionCaptureStepDistance);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Occlusion Mask Fade Hardness", OcclusionMaskFadeHardness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Slope Angle", WetnessSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Slope Smoothness", WetnessSlopeSmoothness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Scale", PuddlesMaskScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Roughness", PuddlesRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Slope Angle", PuddlesSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Slope Smoothness", PuddlesSlopeSmoothness);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Scale", RainRipplesScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Max Density", RainRipplesMaxDensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wind Ripples Scale", WindRipplesScale);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Mask Scale", SnowMaskScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Scale", SnowScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Roughness", SnowRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Sparkles Scale", SnowSparklesScale);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Sparkles Roughness", SnowSparklesRoughness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Slope Angle", SnowSlopeAngle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Slope Smoothness", SnowSlopeSmoothness);
	}
}

void ASkyCreator::SetSkyAtmosphereSettings(FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettings)
{
	if (SkyAtmosphere)
	{ 
		WeatherSettings.SkyAtmosphereSettings = InSkyAtmosphereSettings;

		SkyAtmosphere->SetRayleighScatteringScale(WeatherSettings.SkyAtmosphereSettings.RayleighScatteringScale);
		SkyAtmosphere->SetRayleighScattering(WeatherSettings.SkyAtmosphereSettings.RayleighScattering);
		SkyAtmosphere->SetRayleighExponentialDistribution(WeatherSettings.SkyAtmosphereSettings.RayleighExponentialDistribution);
		SkyAtmosphere->SetMieScatteringScale(WeatherSettings.SkyAtmosphereSettings.MieScatteringScale);
		SkyAtmosphere->SetMieScattering(WeatherSettings.SkyAtmosphereSettings.MieScattering);
		SkyAtmosphere->SetMieAbsorptionScale(WeatherSettings.SkyAtmosphereSettings.MieAbsorptionScale);
		SkyAtmosphere->SetMieAbsorption(WeatherSettings.SkyAtmosphereSettings.MieAbsorption);
		SkyAtmosphere->SetMieAnisotropy(WeatherSettings.SkyAtmosphereSettings.MieAnisotropy);
		SkyAtmosphere->SetMieExponentialDistribution(WeatherSettings.SkyAtmosphereSettings.MieExponentialDistribution);
		SkyAtmosphere->SetOtherAbsorptionScale(WeatherSettings.SkyAtmosphereSettings.AbsorptionScale);
		SkyAtmosphere->SetOtherAbsorption(WeatherSettings.SkyAtmosphereSettings.Absorption);
		SkyAtmosphere->SetSkyLuminanceFactor(WeatherSettings.SkyAtmosphereSettings.SkyLuminanceFactor);
		SkyAtmosphere->GroundAlbedo = WeatherSettings.SkyAtmosphereSettings.GroundAlbedo.ToFColor(true);
		SkyAtmosphere->SetHeightFogContribution(WeatherSettings.SkyAtmosphereSettings.HeightFogContribution);
		SkyAtmosphere->MarkRenderStateDirty();
	}
}

void ASkyCreator::SetVolumetricCloudSettings(FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettings)
{
	if (VolumetricCloud && VolumetricCloudMID && VolumetricCloudsMPC)
	{
		WeatherSettings.VolumetricCloudSettings = InVolumetricCloudSettings;

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratus Coverage", WeatherSettings.VolumetricCloudSettings.StratusCoverage);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratus Coverage Variation", WeatherSettings.VolumetricCloudSettings.StratusCoverageVariation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratus Height Variation", WeatherSettings.VolumetricCloudSettings.StratusHeightVariation);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratocumulus Coverage", WeatherSettings.VolumetricCloudSettings.StratocumulusCoverage);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratocumulus Coverage Variation", WeatherSettings.VolumetricCloudSettings.StratocumulusCoverageVariation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Stratocumulus Height Variation", WeatherSettings.VolumetricCloudSettings.StratocumulusHeightVariation);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulus Coverage", WeatherSettings.VolumetricCloudSettings.CumulusCoverage);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulus Coverage Variation", WeatherSettings.VolumetricCloudSettings.CumulusCoverageVariation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulus Height Variation", WeatherSettings.VolumetricCloudSettings.CumulusHeightVariation);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulonimbus Coverage", WeatherSettings.VolumetricCloudSettings.CumulonimbusCoverage);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulonimbus Anvil", WeatherSettings.VolumetricCloudSettings.CumulonimbusAnvil);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Cumulonimbus Height Variation", WeatherSettings.VolumetricCloudSettings.CumulonimbusHeightVariation);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Density Bottom", WeatherSettings.VolumetricCloudSettings.DensityBottom);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Density Middle", WeatherSettings.VolumetricCloudSettings.DensityMiddle);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Density Top", WeatherSettings.VolumetricCloudSettings.DensityTop);

		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Albedo", WeatherSettings.VolumetricCloudSettings.Albedo);
		VolumetricCloud->SetGroundAlbedo(WeatherSettings.VolumetricCloudSettings.GroundAlbedo.ToFColor(true));
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Beer's Powder Intensity", WeatherSettings.VolumetricCloudSettings.BeersPowderIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Beer's Powder Depth", WeatherSettings.VolumetricCloudSettings.BeersPowderDepth);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Bottom Occlusion", WeatherSettings.VolumetricCloudSettings.BottomOcclusion);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Bottom Occlusion Height", WeatherSettings.VolumetricCloudSettings.BottomOcclusionHeight);
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Night Emissive", WeatherSettings.VolumetricCloudSettings.NightEmissive);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Phase G", WeatherSettings.VolumetricCloudSettings.PhaseG);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Phase G2", WeatherSettings.VolumetricCloudSettings.PhaseG2);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Phase Blend", WeatherSettings.VolumetricCloudSettings.PhaseBlend);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Multi Scattering Contribution", WeatherSettings.VolumetricCloudSettings.MultiScatteringContribution);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Multi Scattering Occlusion", WeatherSettings.VolumetricCloudSettings.MultiScatteringOcclusion);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Multi Scattering Eccentricity", WeatherSettings.VolumetricCloudSettings.MultiScatteringEccentricity);

		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Shape Intensity A", WeatherSettings.VolumetricCloudSettings.NoiseShapeIntensityA);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Shape Intensity B", WeatherSettings.VolumetricCloudSettings.NoiseShapeIntensityB);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Shape Intensity C", WeatherSettings.VolumetricCloudSettings.NoiseShapeIntensityC);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Shape Intensity D", WeatherSettings.VolumetricCloudSettings.NoiseShapeIntensityD);
//		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Detail Intensity A", WeatherSettings.VolumetricCloudSettings.NoiseDetailIntensityA);
//		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Detail Intensity B", WeatherSettings.VolumetricCloudSettings.NoiseDetailIntensityB);
//		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Noise Detail Intensity C", WeatherSettings.VolumetricCloudSettings.NoiseDetailIntensityC);
		UKismetMaterialLibrary::SetScalarParameterValue(this, VolumetricCloudsMPC, "Turbulence Intensity", WeatherSettings.VolumetricCloudSettings.TurbulenceIntensity);
	}
}

void ASkyCreator::SetBackgroundCloudSettings(FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettings)
{
	if (SkySphere && CommonMPC)
	{
		WeatherSettings.BackgroundCloudSettings = InBackgroundCloudSettings;

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Intensity", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsIntensity);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Background Clouds Color Tint", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsColorTint);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Layer A", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsLayerA);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Layer B", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsLayerB);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Layer C", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsLayerC);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Background Clouds Phase", WeatherSettings.BackgroundCloudSettings.BackgroundCloudsLightningPhase);
	}
}

void ASkyCreator::SetSkyLightSettings(FSkyCreatorSkyLightSettings InSkyLightSettings)
{
	if (SkyLight && CommonMPC)
	{
		WeatherSettings.SkyLightSettings = InSkyLightSettings;

		SkyLight->SetIntensity(WeatherSettings.SkyLightSettings.Intensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Sky Light Night Intensity", WeatherSettings.SkyLightSettings.NightIntensity);
		SkyLight->SetLightColor(WeatherSettings.SkyLightSettings.LightColor);
		SkyLight->SetLowerHemisphereColor(WeatherSettings.SkyLightSettings.LowerHemisphereColor);
		SkyLight->CloudAmbientOcclusionStrength = WeatherSettings.SkyLightSettings.CloudAmbientOcclusionStrength;
		SkyLight->MarkRenderStateDirty();
		//SkySphereMID->SetScalarParameterValue("SkyLight_DayIntensity", WeatherSettings.SkyLightSettings.DayIntensity);
		//SkySphereMID->SetScalarParameterValue("SkyLight_NightIntensity", WeatherSettings.SkyLightSettings.NightIntensity);
	}
}

void ASkyCreator::SetSunLightSettings(FSkyCreatorSunLightSettings InSunLightSettings)
{
	if (SunLight)
	{
		WeatherSettings.SunLightSettings = InSunLightSettings;

		if (!bSunConstantIntensity)
		{
			SunLight->SetIntensity(WeatherSettings.SunLightSettings.Intensity);
		}

		if (bSunConstantAtmosphereDiskColorScale)
		{
			WeatherSettings.SunLightSettings.AtmosphereDiskColorScale = SunAtmosphereDiskColorScale;
		}
		
		//SunLight->SetIntensity(WeatherSettings.SunLightSettings.Intensity);
		SunLight->SetLightColor(WeatherSettings.SunLightSettings.LightColor);
		SunLight->SetTemperature(WeatherSettings.SunLightSettings.Temperature);
		SunLight->SetVolumetricScatteringIntensity(WeatherSettings.SunLightSettings.VolumetricScatteringIntensity);
		SunLight->AtmosphereSunDiskColorScale = WeatherSettings.SunLightSettings.AtmosphereDiskColorScale;
		SunLight->CloudScatteredLuminanceScale = WeatherSettings.SunLightSettings.CloudScatteredLuminanceScale;
		SunLight->CloudShadowStrength = WeatherSettings.SunLightSettings.CloudShadowStrength;
		SunLight->CloudShadowOnAtmosphereStrength = WeatherSettings.SunLightSettings.CloudShadowOnAtmosphereStrength;
		SunLight->CloudShadowOnSurfaceStrength = WeatherSettings.SunLightSettings.CloudShadowOnSurfaceStrength;
		SunLight->MarkRenderStateDirty();
		UpdateSunMoonIntensity(TimeOfDay);
	}
}

void ASkyCreator::SetMoonLightSettings(FSkyCreatorMoonLightSettings InMoonLightSettings)
{
	if (MoonLight && CommonMPC)
	{
		WeatherSettings.MoonLightSettings = InMoonLightSettings;

		if (!bMoonConstantIntensity)
		{
			MoonLight->SetIntensity(WeatherSettings.MoonLightSettings.Intensity);
		}

		if (bMoonConstantAtmosphereDiskColorScale)
		{
			WeatherSettings.MoonLightSettings.AtmosphereDiskColorScale = MoonAtmosphereDiskColorScale;
		}

		//MoonLight->SetIntensity(WeatherSettings.MoonLightSettings.Intensity);
		MoonLight->SetLightColor(WeatherSettings.MoonLightSettings.LightColor);
		MoonLight->SetTemperature(WeatherSettings.MoonLightSettings.Temperature);
		MoonLight->SetVolumetricScatteringIntensity(WeatherSettings.MoonLightSettings.VolumetricScatteringIntensity);
		MoonLight->AtmosphereSunDiskColorScale = WeatherSettings.MoonLightSettings.AtmosphereDiskColorScale;
//		SkySphereMID->SetVectorParameterValue("Moon_Color", WeatherSettings.MoonLightSettings.AtmosphereDiskColorScale);
		MoonLight->CloudScatteredLuminanceScale = WeatherSettings.MoonLightSettings.CloudScatteredLuminanceScale;
		MoonLight->CloudShadowStrength = WeatherSettings.MoonLightSettings.CloudShadowStrength;
		MoonLight->CloudShadowOnAtmosphereStrength = WeatherSettings.MoonLightSettings.CloudShadowOnAtmosphereStrength;
		MoonLight->CloudShadowOnSurfaceStrength = WeatherSettings.MoonLightSettings.CloudShadowOnSurfaceStrength;
		MoonLight->MarkRenderStateDirty();
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Moon Color", WeatherSettings.MoonLightSettings.AtmosphereDiskColorScale);
		UpdateSunMoonIntensity(TimeOfDay);
	}
}

void ASkyCreator::SetExponentialHeightFogSettings(FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettings)
{
	if (ExponentialHeightFog)
	{
		WeatherSettings.ExponentialHeightFogSettings = InExponentialHeightFogSettings;

		ExponentialHeightFog->SetFogDensity(WeatherSettings.ExponentialHeightFogSettings.FogDensity);
		ExponentialHeightFog->SetFogHeightFalloff(WeatherSettings.ExponentialHeightFogSettings.FogHeightFalloff);
		ExponentialHeightFog->SetFogInscatteringColor(WeatherSettings.ExponentialHeightFogSettings.FogInscatteringColor);
		ExponentialHeightFog->SetStartDistance(WeatherSettings.ExponentialHeightFogSettings.FogStartDistance);

		ExponentialHeightFog->SecondFogData.FogDensity = WeatherSettings.ExponentialHeightFogSettings.SecondFogDensity;
		ExponentialHeightFog->SecondFogData.FogHeightFalloff = WeatherSettings.ExponentialHeightFogSettings.SecondFogHeightFalloff;

		ExponentialHeightFog->SetDirectionalInscatteringExponent(WeatherSettings.ExponentialHeightFogSettings.DirectionalInscatteringExponent);
		ExponentialHeightFog->SetDirectionalInscatteringColor(WeatherSettings.ExponentialHeightFogSettings.DirectionalInscatteringColor);
		ExponentialHeightFog->SetDirectionalInscatteringStartDistance(WeatherSettings.ExponentialHeightFogSettings.DirectionalInscatteringStartDistance);

		ExponentialHeightFog->SetVolumetricFogScatteringDistribution(WeatherSettings.ExponentialHeightFogSettings.VolumetricFogScatteringDistribution);
		ExponentialHeightFog->SetVolumetricFogAlbedo(WeatherSettings.ExponentialHeightFogSettings.VolumetricFogAlbedo.ToFColor(true));
		ExponentialHeightFog->SetVolumetricFogEmissive(WeatherSettings.ExponentialHeightFogSettings.VolumetricFogEmissive);
		ExponentialHeightFog->SetVolumetricFogExtinctionScale(WeatherSettings.ExponentialHeightFogSettings.VolumetricFogExtinctionScale);
		ExponentialHeightFog->MarkRenderStateDirty();
	}
}

void ASkyCreator::SetStarMapSettings(FSkyCreatorStarMapSettings InStarMapSettings)
{
	if (SkySphere && CommonMPC)
	{
		WeatherSettings.StarMapSettings = InStarMapSettings;

/*		SkySphereMID->SetScalarParameterValue("StarMap_Brightness", WeatherSettings.StarMapSettings.StarMapIntensity);
		SkySphereMID->SetVectorParameterValue("StarMap_ColorTint", WeatherSettings.StarMapSettings.StarMapColorTint);
		SkySphereMID->SetScalarParameterValue("StarMap_TwinkleIntensity", WeatherSettings.StarMapSettings.StarMapTwinkleIntensity);
		SkySphereMID->SetScalarParameterValue("StarMap_TwinkleSaturation", WeatherSettings.StarMapSettings.StarMapTwinkleSaturation);
		SkySphereMID->SetScalarParameterValue("StarMap_TwinkleSpeed", WeatherSettings.StarMapSettings.StarMapTwinkleSpeed);
		SkySphereMID->SetScalarParameterValue("StarMap_HorizontalMask_Multiply", WeatherSettings.StarMapSettings.StarMapHorizonThreshold);
		SkySphereMID->SetScalarParameterValue("StarMap_SkyAtmosphereMask_Multiplier", WeatherSettings.StarMapSettings.StarMapAtmosphereThreshold);
		SkySphereMID->SetVectorParameterValue("Night_ZenithColor", WeatherSettings.StarMapSettings.NightZenithColor); // Fake Night Atmosphere Zenith Color
		SkySphereMID->SetVectorParameterValue("Night_HorizonColor", WeatherSettings.StarMapSettings.NightHorizonColor); // Fake Night Atmosphere Horizon Color
*/
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Intensity", WeatherSettings.StarMapSettings.StarMapIntensity);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Star Map Color Tint", WeatherSettings.StarMapSettings.StarMapColorTint);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Twinkle Intensity", WeatherSettings.StarMapSettings.StarMapTwinkleIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Twinkle Saturation", WeatherSettings.StarMapSettings.StarMapTwinkleSaturation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Twinkle Speed", WeatherSettings.StarMapSettings.StarMapTwinkleSpeed);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Horizon Threshold", WeatherSettings.StarMapSettings.StarMapHorizonThreshold);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Star Map Atmosphere Threshold", WeatherSettings.StarMapSettings.StarMapAtmosphereThreshold);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Night Zenith Color", WeatherSettings.StarMapSettings.NightZenithColor); // Fake Night Atmosphere Zenith Color
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Night Horizon Color", WeatherSettings.StarMapSettings.NightHorizonColor); // Fake Night Atmosphere Horizon Color
	}
}

void ASkyCreator::SetWeatherFXSettings(FSkyCreatorWeatherFXSettings InWeatherFXSettings)
{
	if (WeatherFX)
	{
		WeatherSettings.WeatherFXSettings = InWeatherFXSettings;

		WeatherFX->SetNiagaraVariableFloat("Rain Spawn Rate", FMath::Lerp(0.0f, RainSpawnRateMaxCPU, WeatherSettings.WeatherFXSettings.RainAmount));
		WeatherFX->SetNiagaraVariableFloat("Rain Spawn Rate GPU", FMath::Lerp(0.0f, RainSpawnRateMaxGPU, WeatherSettings.WeatherFXSettings.RainAmount));
		WeatherFX->SetNiagaraVariableLinearColor("Rain Color", WeatherSettings.WeatherFXSettings.RainColor);
		WeatherFX->SetNiagaraVariableFloat("Rain Gravity", WeatherSettings.WeatherFXSettings.RainGravity);
		WeatherFX->SetNiagaraVariableFloat("Rain Lifetime Min", WeatherSettings.WeatherFXSettings.RainLifetimeMin);
		WeatherFX->SetNiagaraVariableFloat("Rain Lifetime Max", WeatherSettings.WeatherFXSettings.RainLifetimeMax);
		WeatherFX->SetNiagaraVariableFloat("Rain Size Min", WeatherSettings.WeatherFXSettings.RainSizeMin);
		WeatherFX->SetNiagaraVariableFloat("Rain Size Max", WeatherSettings.WeatherFXSettings.RainSizeMax);

		WeatherFX->SetNiagaraVariableFloat("Rain Splash Lifetime Min", WeatherSettings.WeatherFXSettings.RainSplashLifetimeMin);
		WeatherFX->SetNiagaraVariableFloat("Rain Splash Lifetime Max", WeatherSettings.WeatherFXSettings.RainSplashLifetimeMax);
		WeatherFX->SetNiagaraVariableVec2("Rain Splash Min Size", WeatherSettings.WeatherFXSettings.RainSplashSizeMin);
		WeatherFX->SetNiagaraVariableVec2("Rain Splash Max Size", WeatherSettings.WeatherFXSettings.RainSplashSizeMax);

//		WeatherFX->SetNiagaraVariableFloat("Rain Splash Spawn Rate", FMath::Lerp(0.0f, RainSplashSpawnRateMax, WeatherSettings.WeatherFXSettings.RainAmount));
		WeatherFX->SetNiagaraVariableFloat("Rain Splash Spawn Rate GPU", FMath::Lerp(0.0f, RainSplashSpawnRateMaxGPU, WeatherSettings.WeatherFXSettings.RainAmount));
//		WeatherFX->SetColorParameter("Rain Splash Color", WeatherSettings.WeatherFXSettings.RainSplashColor);

		WeatherFX->SetNiagaraVariableFloat("Snow Spawn Rate", FMath::Lerp(0.0f, SnowSpawnRateMaxCPU, WeatherSettings.WeatherFXSettings.SnowAmount));
		WeatherFX->SetNiagaraVariableFloat("Snow Spawn Rate GPU", FMath::Lerp(0.0f, SnowSpawnRateMaxGPU, WeatherSettings.WeatherFXSettings.SnowAmount));
		WeatherFX->SetNiagaraVariableLinearColor("Snow Color", WeatherSettings.WeatherFXSettings.SnowColor);
		WeatherFX->SetNiagaraVariableFloat("Snow Gravity", WeatherSettings.WeatherFXSettings.SnowGravity);
		WeatherFX->SetNiagaraVariableFloat("Snow Turbulence", WeatherSettings.WeatherFXSettings.SnowTurbulence);
		WeatherFX->SetNiagaraVariableFloat("Snow Lifetime Min", WeatherSettings.WeatherFXSettings.SnowLifetimeMin);
		WeatherFX->SetNiagaraVariableFloat("Snow Lifetime Max", WeatherSettings.WeatherFXSettings.SnowLifetimeMax);
		WeatherFX->SetNiagaraVariableFloat("Snow Size Min", WeatherSettings.WeatherFXSettings.SnowSizeMin);
		WeatherFX->SetNiagaraVariableFloat("Snow Size Max", WeatherSettings.WeatherFXSettings.SnowSizeMax);

		WeatherFX->SetNiagaraVariableFloat("Rainbow Amount", WeatherSettings.WeatherFXSettings.RainbowAmount);
		WeatherFX->SetNiagaraVariableLinearColor("Rainbow Color", WeatherSettings.WeatherFXSettings.RainbowColor);
		WeatherFX->SetNiagaraVariableFloat("Rainbow Radius", WeatherSettings.WeatherFXSettings.RainbowRadius);
		WeatherFX->SetNiagaraVariableFloat("Rainbow Thickness", WeatherSettings.WeatherFXSettings.RainbowThickness);
		WeatherFX->SetNiagaraVariableLinearColor("Secondary Rainbow Color", WeatherSettings.WeatherFXSettings.SecondaryRainbowColor);
		WeatherFX->SetNiagaraVariableFloat("Secondary Rainbow Radius", WeatherSettings.WeatherFXSettings.SecondaryRainbowRadius);
		WeatherFX->SetNiagaraVariableFloat("Secondary Rainbow Thickness", WeatherSettings.WeatherFXSettings.SecondaryRainbowThickness);
	}
}

void ASkyCreator::SetWeatherMaterialFXSettings(FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettings)
{
	if (CommonMPC)
	{
		WeatherSettings.WeatherMaterialFXSettings = InWeatherMaterialFXSettings;

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Amount", WeatherSettings.WeatherMaterialFXSettings.WetnessAmount);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Wetness Color", WeatherSettings.WeatherMaterialFXSettings.WetnessColor);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Amount", WeatherSettings.WeatherMaterialFXSettings.PuddlesAmount);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Puddles Color", WeatherSettings.WeatherMaterialFXSettings.PuddlesColor);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Amount", WeatherSettings.WeatherMaterialFXSettings.RainRipplesAmount);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Intensity", WeatherSettings.WeatherMaterialFXSettings.RainRipplesIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Force Min Radius", WeatherSettings.WeatherMaterialFXSettings.RainRipplesForceMinRadius);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Force Max Radius", WeatherSettings.WeatherMaterialFXSettings.RainRipplesForceMaxRadius);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Rings Number", WeatherSettings.WeatherMaterialFXSettings.RainRipplesRingsNumber);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Rings Delay", WeatherSettings.WeatherMaterialFXSettings.RainRipplesRingsDelay);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Rain Ripples Damping", WeatherSettings.WeatherMaterialFXSettings.RainRipplesDamping);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wind Ripples Intensity", WeatherSettings.WeatherMaterialFXSettings.WindRipplesIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wind Ripples Speed", WeatherSettings.WeatherMaterialFXSettings.WindRipplesSpeed);

		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Amount", WeatherSettings.WeatherMaterialFXSettings.SnowAmount);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Snow Color", WeatherSettings.WeatherMaterialFXSettings.SnowColor);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Intensity", WeatherSettings.WeatherMaterialFXSettings.SnowIntensity);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Sparkles Threshold", WeatherSettings.WeatherMaterialFXSettings.SnowSparklesThreshold);
	}
}

void ASkyCreator::SetWindSettings(FSkyCreatorWindSettings InWindSettings)
{
	if (VolumetricCloud && VolumetricCloudMID && WeatherFX)
	{
		if (!bIndependentWindControl)
		{
			WeatherSettings.WindSettings = InWindSettings;

			WeatherFX->SetFloatParameter("Wind Direction", WeatherSettings.WindSettings.PrecipitationWindDirection);
			WeatherFX->SetFloatParameter("Wind Speed", WeatherSettings.WindSettings.PrecipitationWindSpeed);
		}

		CloudWindSkewForce = CloudWindSkewAmount * (WeatherSettings.WindSettings.CloudWindSpeed / 100.0f);
	}
}

void ASkyCreator::SetWindIndependentSettings(FSkyCreatorWindSettings InWindSettings)
{
	if (VolumetricCloud && VolumetricCloudMID && WeatherFX)
	{
		if (bIndependentWindControl)
		{
			WeatherSettings.WindSettings = InWindSettings;

			WeatherFX->SetFloatParameter("Wind Direction", WeatherSettings.WindSettings.PrecipitationWindDirection);
			WeatherFX->SetFloatParameter("Wind Speed", WeatherSettings.WindSettings.PrecipitationWindSpeed);
		}
	}
}

void ASkyCreator::SetPostProcessSettings(FSkyCreatorPostProcessSettings InPostProcessSettings)
{
	if (PostProcess)
	{
		WeatherSettings.PostProcessSettings = InPostProcessSettings;

		PostProcess->Settings.BloomIntensity = WeatherSettings.PostProcessSettings.BloomIntensity;
		PostProcess->Settings.BloomThreshold = WeatherSettings.PostProcessSettings.BloomThreshold;
		PostProcess->Settings.AutoExposureBias = WeatherSettings.PostProcessSettings.ExposureCompensation;
	}
}

void ASkyCreator::SetWeatherSettings(FSkyCreatorWeatherSettings InWeatherSettings)
{
	SetSkyAtmosphereSettings(InWeatherSettings.SkyAtmosphereSettings);
	SetVolumetricCloudSettings(InWeatherSettings.VolumetricCloudSettings);
	SetBackgroundCloudSettings(InWeatherSettings.BackgroundCloudSettings);
	SetSkyLightSettings(InWeatherSettings.SkyLightSettings);
	SetSunLightSettings(InWeatherSettings.SunLightSettings);
	SetMoonLightSettings(InWeatherSettings.MoonLightSettings);
	SetExponentialHeightFogSettings(InWeatherSettings.ExponentialHeightFogSettings);
	SetStarMapSettings(InWeatherSettings.StarMapSettings);
	SetWeatherFXSettings(InWeatherSettings.WeatherFXSettings);
	SetWeatherMaterialFXSettings(InWeatherSettings.WeatherMaterialFXSettings);
	SetWindSettings(InWeatherSettings.WindSettings);
	SetPostProcessSettings(InWeatherSettings.PostProcessSettings);
}

void ASkyCreator::LerpSkyAtmosphereSettings(FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettingsA, FSkyCreatorSkyAtmosphereSettings InSkyAtmosphereSettingsB, float Alpha)
{
	if (SkyAtmosphere)
	{
		FSkyCreatorSkyAtmosphereSettings SkyAtmosphereSettings;

		SkyAtmosphereSettings.RayleighScatteringScale = FMath::Lerp(InSkyAtmosphereSettingsA.RayleighScatteringScale, InSkyAtmosphereSettingsB.RayleighScatteringScale, Alpha);
		SkyAtmosphereSettings.RayleighScattering = FMath::Lerp(InSkyAtmosphereSettingsA.RayleighScattering, InSkyAtmosphereSettingsB.RayleighScattering, Alpha);
		SkyAtmosphereSettings.RayleighExponentialDistribution = FMath::Lerp(InSkyAtmosphereSettingsA.RayleighExponentialDistribution, InSkyAtmosphereSettingsB.RayleighExponentialDistribution, Alpha);
		SkyAtmosphereSettings.MieScatteringScale = FMath::Lerp(InSkyAtmosphereSettingsA.MieScatteringScale, InSkyAtmosphereSettingsB.MieScatteringScale, Alpha);
		SkyAtmosphereSettings.MieScattering = FMath::Lerp(InSkyAtmosphereSettingsA.MieScattering, InSkyAtmosphereSettingsB.MieScattering, Alpha);
		SkyAtmosphereSettings.MieAbsorptionScale = FMath::Lerp(InSkyAtmosphereSettingsA.MieAbsorptionScale, InSkyAtmosphereSettingsB.MieAbsorptionScale, Alpha);
		SkyAtmosphereSettings.MieAbsorption = FMath::Lerp(InSkyAtmosphereSettingsA.MieAbsorption, InSkyAtmosphereSettingsB.MieAbsorption, Alpha);
		SkyAtmosphereSettings.MieAnisotropy = FMath::Lerp(InSkyAtmosphereSettingsA.MieAnisotropy, InSkyAtmosphereSettingsB.MieAnisotropy, Alpha);
		SkyAtmosphereSettings.MieExponentialDistribution = FMath::Lerp(InSkyAtmosphereSettingsA.MieExponentialDistribution, InSkyAtmosphereSettingsB.MieExponentialDistribution, Alpha);
		SkyAtmosphereSettings.AbsorptionScale = FMath::Lerp(InSkyAtmosphereSettingsA.AbsorptionScale, InSkyAtmosphereSettingsB.AbsorptionScale, Alpha);
		SkyAtmosphereSettings.Absorption = FMath::Lerp(InSkyAtmosphereSettingsA.Absorption, InSkyAtmosphereSettingsB.Absorption, Alpha);
		SkyAtmosphereSettings.SkyLuminanceFactor = FMath::Lerp(InSkyAtmosphereSettingsA.SkyLuminanceFactor, InSkyAtmosphereSettingsB.SkyLuminanceFactor, Alpha);
		SkyAtmosphereSettings.GroundAlbedo = FMath::Lerp(InSkyAtmosphereSettingsA.GroundAlbedo, InSkyAtmosphereSettingsB.GroundAlbedo, Alpha);
		SkyAtmosphereSettings.HeightFogContribution = FMath::Lerp(InSkyAtmosphereSettingsA.HeightFogContribution, InSkyAtmosphereSettingsB.HeightFogContribution, Alpha);

		SetSkyAtmosphereSettings(SkyAtmosphereSettings);
	}
}

void ASkyCreator::LerpVolumetricCloudSettings(FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettingsA, FSkyCreatorVolumetricCloudSettings InVolumetricCloudSettingsB, float Alpha)
{
	if (VolumetricCloud && VolumetricCloudMID)
	{
		FSkyCreatorVolumetricCloudSettings VolumetricCloudSettings;

		VolumetricCloudSettings.StratusCoverage = FMath::Lerp(InVolumetricCloudSettingsA.StratusCoverage, InVolumetricCloudSettingsB.StratusCoverage, Alpha);
		VolumetricCloudSettings.StratusCoverageVariation = FMath::Lerp(InVolumetricCloudSettingsA.StratusCoverageVariation, InVolumetricCloudSettingsB.StratusCoverageVariation, Alpha);
		VolumetricCloudSettings.StratusHeightVariation = FMath::Lerp(InVolumetricCloudSettingsA.StratusHeightVariation, InVolumetricCloudSettingsB.StratusHeightVariation, Alpha);

		VolumetricCloudSettings.StratocumulusCoverage = FMath::Lerp(InVolumetricCloudSettingsA.StratocumulusCoverage, InVolumetricCloudSettingsB.StratocumulusCoverage, Alpha);
		VolumetricCloudSettings.StratocumulusCoverageVariation = FMath::Lerp(InVolumetricCloudSettingsA.StratocumulusCoverageVariation, InVolumetricCloudSettingsB.StratocumulusCoverageVariation, Alpha);
		VolumetricCloudSettings.StratocumulusHeightVariation = FMath::Lerp(InVolumetricCloudSettingsA.StratocumulusHeightVariation, InVolumetricCloudSettingsB.StratocumulusHeightVariation, Alpha);

		VolumetricCloudSettings.CumulusCoverage = FMath::Lerp(InVolumetricCloudSettingsA.CumulusCoverage, InVolumetricCloudSettingsB.CumulusCoverage, Alpha);
		VolumetricCloudSettings.CumulusCoverageVariation = FMath::Lerp(InVolumetricCloudSettingsA.CumulusCoverageVariation, InVolumetricCloudSettingsB.CumulusCoverageVariation, Alpha);
		VolumetricCloudSettings.CumulusHeightVariation = FMath::Lerp(InVolumetricCloudSettingsA.CumulusHeightVariation, InVolumetricCloudSettingsB.CumulusHeightVariation, Alpha);

		VolumetricCloudSettings.CumulonimbusCoverage = FMath::Lerp(InVolumetricCloudSettingsA.CumulonimbusCoverage, InVolumetricCloudSettingsB.CumulonimbusCoverage, Alpha);
		VolumetricCloudSettings.CumulonimbusAnvil = FMath::Lerp(InVolumetricCloudSettingsA.CumulonimbusAnvil, InVolumetricCloudSettingsB.CumulonimbusAnvil, Alpha);
		VolumetricCloudSettings.CumulonimbusHeightVariation = FMath::Lerp(InVolumetricCloudSettingsA.CumulonimbusHeightVariation, InVolumetricCloudSettingsB.CumulonimbusHeightVariation, Alpha);

		VolumetricCloudSettings.DensityBottom = FMath::Lerp(InVolumetricCloudSettingsA.DensityBottom, InVolumetricCloudSettingsB.DensityBottom, Alpha);
		VolumetricCloudSettings.DensityMiddle = FMath::Lerp(InVolumetricCloudSettingsA.DensityMiddle, InVolumetricCloudSettingsB.DensityMiddle, Alpha);
		VolumetricCloudSettings.DensityTop = FMath::Lerp(InVolumetricCloudSettingsA.DensityTop, InVolumetricCloudSettingsB.DensityTop, Alpha);

		VolumetricCloudSettings.Albedo = FMath::Lerp(InVolumetricCloudSettingsA.Albedo, InVolumetricCloudSettingsB.Albedo, Alpha);
		VolumetricCloudSettings.GroundAlbedo = FMath::Lerp(InVolumetricCloudSettingsA.GroundAlbedo, InVolumetricCloudSettingsB.GroundAlbedo, Alpha);
		VolumetricCloudSettings.BeersPowderIntensity = FMath::Lerp(InVolumetricCloudSettingsA.BeersPowderIntensity, InVolumetricCloudSettingsB.BeersPowderIntensity, Alpha);
		VolumetricCloudSettings.BeersPowderDepth = FMath::Lerp(InVolumetricCloudSettingsA.BeersPowderDepth, InVolumetricCloudSettingsB.BeersPowderDepth, Alpha);
		VolumetricCloudSettings.BottomOcclusion = FMath::Lerp(InVolumetricCloudSettingsA.BottomOcclusion, InVolumetricCloudSettingsB.BottomOcclusion, Alpha);
		VolumetricCloudSettings.BottomOcclusionHeight = FMath::Lerp(InVolumetricCloudSettingsA.BottomOcclusionHeight, InVolumetricCloudSettingsB.BottomOcclusionHeight, Alpha);
		VolumetricCloudSettings.NightEmissive = FMath::Lerp(InVolumetricCloudSettingsA.NightEmissive, InVolumetricCloudSettingsB.NightEmissive, Alpha);
		VolumetricCloudSettings.PhaseG = FMath::Lerp(InVolumetricCloudSettingsA.PhaseG, InVolumetricCloudSettingsB.PhaseG, Alpha);
		VolumetricCloudSettings.PhaseG2 = FMath::Lerp(InVolumetricCloudSettingsA.PhaseG2, InVolumetricCloudSettingsB.PhaseG2, Alpha);
		VolumetricCloudSettings.PhaseBlend = FMath::Lerp(InVolumetricCloudSettingsA.PhaseBlend, InVolumetricCloudSettingsB.PhaseBlend, Alpha);
		VolumetricCloudSettings.MultiScatteringContribution = FMath::Lerp(InVolumetricCloudSettingsA.MultiScatteringContribution, InVolumetricCloudSettingsB.MultiScatteringContribution, Alpha);
		VolumetricCloudSettings.MultiScatteringOcclusion = FMath::Lerp(InVolumetricCloudSettingsA.MultiScatteringOcclusion, InVolumetricCloudSettingsB.MultiScatteringOcclusion, Alpha);
		VolumetricCloudSettings.MultiScatteringEccentricity = FMath::Lerp(InVolumetricCloudSettingsA.MultiScatteringEccentricity, InVolumetricCloudSettingsB.MultiScatteringEccentricity, Alpha);

		VolumetricCloudSettings.NoiseShapeIntensityA = FMath::Lerp(InVolumetricCloudSettingsA.NoiseShapeIntensityA, InVolumetricCloudSettingsB.NoiseShapeIntensityA, Alpha);
		VolumetricCloudSettings.NoiseShapeIntensityB = FMath::Lerp(InVolumetricCloudSettingsA.NoiseShapeIntensityB, InVolumetricCloudSettingsB.NoiseShapeIntensityB, Alpha);
		VolumetricCloudSettings.NoiseShapeIntensityC = FMath::Lerp(InVolumetricCloudSettingsA.NoiseShapeIntensityC, InVolumetricCloudSettingsB.NoiseShapeIntensityC, Alpha);
		VolumetricCloudSettings.NoiseShapeIntensityD = FMath::Lerp(InVolumetricCloudSettingsA.NoiseShapeIntensityD, InVolumetricCloudSettingsB.NoiseShapeIntensityD, Alpha);
//		VolumetricCloudSettings.NoiseDetailIntensityA = FMath::Lerp(InVolumetricCloudSettingsA.NoiseDetailIntensityA, InVolumetricCloudSettingsB.NoiseDetailIntensityA, Alpha);
//		VolumetricCloudSettings.NoiseDetailIntensityB = FMath::Lerp(InVolumetricCloudSettingsA.NoiseDetailIntensityB, InVolumetricCloudSettingsB.NoiseDetailIntensityB, Alpha);
//		VolumetricCloudSettings.NoiseDetailIntensityC = FMath::Lerp(InVolumetricCloudSettingsA.NoiseDetailIntensityC, InVolumetricCloudSettingsB.NoiseDetailIntensityC, Alpha);
		VolumetricCloudSettings.TurbulenceIntensity = FMath::Lerp(InVolumetricCloudSettingsA.TurbulenceIntensity, InVolumetricCloudSettingsB.TurbulenceIntensity, Alpha);

		SetVolumetricCloudSettings(VolumetricCloudSettings);
	}
}

void ASkyCreator::LerpBackgroundCloudSettings(FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettingsA, FSkyCreatorBackgroundCloudSettings InBackgroundCloudSettingsB, float Alpha)
{
	if (SkySphere)
	{
		FSkyCreatorBackgroundCloudSettings BackgroundCloudSettings;

		BackgroundCloudSettings.BackgroundCloudsIntensity = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsIntensity, InBackgroundCloudSettingsB.BackgroundCloudsIntensity, Alpha);
		BackgroundCloudSettings.BackgroundCloudsColorTint = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsColorTint, InBackgroundCloudSettingsB.BackgroundCloudsColorTint, Alpha);
		BackgroundCloudSettings.BackgroundCloudsLayerA = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsLayerA, InBackgroundCloudSettingsB.BackgroundCloudsLayerA, Alpha);
		BackgroundCloudSettings.BackgroundCloudsLayerB = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsLayerB, InBackgroundCloudSettingsB.BackgroundCloudsLayerB, Alpha);
		BackgroundCloudSettings.BackgroundCloudsLayerC = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsLayerC, InBackgroundCloudSettingsB.BackgroundCloudsLayerC, Alpha);
		BackgroundCloudSettings.BackgroundCloudsLightningPhase = FMath::Lerp(InBackgroundCloudSettingsA.BackgroundCloudsLightningPhase, InBackgroundCloudSettingsB.BackgroundCloudsLightningPhase, Alpha);

		SetBackgroundCloudSettings(BackgroundCloudSettings);
	}
}

void ASkyCreator::LerpSkyLightSettings(FSkyCreatorSkyLightSettings InSkyLightSettingsA, FSkyCreatorSkyLightSettings InSkyLightSettingsB, float Alpha)
{
	if (SkyLight)
	{
		FSkyCreatorSkyLightSettings SkyLightSettings;

		SkyLightSettings.Intensity = FMath::Lerp(InSkyLightSettingsA.Intensity, InSkyLightSettingsB.Intensity, Alpha);
		SkyLightSettings.NightIntensity = FMath::Lerp(InSkyLightSettingsA.NightIntensity, InSkyLightSettingsB.NightIntensity, Alpha);
		SkyLightSettings.LightColor = FMath::Lerp(InSkyLightSettingsA.LightColor, InSkyLightSettingsB.LightColor, Alpha);
		SkyLightSettings.LowerHemisphereColor = FMath::Lerp(InSkyLightSettingsA.LowerHemisphereColor, InSkyLightSettingsB.LowerHemisphereColor, Alpha);
		SkyLightSettings.CloudAmbientOcclusionStrength = FMath::Lerp(InSkyLightSettingsA.CloudAmbientOcclusionStrength, InSkyLightSettingsB.CloudAmbientOcclusionStrength, Alpha);
		//SkyLightSettings.DayIntensity = FMath::Lerp(InSkyLightSettingsA.DayIntensity, InSkyLightSettingsB.DayIntensity, Alpha);
		
		SetSkyLightSettings(SkyLightSettings);
	}
}

void ASkyCreator::LerpSunLightSettings(FSkyCreatorSunLightSettings InSunLightSettingsA, FSkyCreatorSunLightSettings InSunLightSettingsB, float Alpha)
{
	if (SunLight)
	{
		FSkyCreatorSunLightSettings SunLightSettings;

		if (!bSunConstantIntensity) 
		{
			SunLightSettings.Intensity = FMath::Lerp(InSunLightSettingsA.Intensity, InSunLightSettingsB.Intensity, Alpha);
		}
		SunLightSettings.LightColor = FMath::Lerp(InSunLightSettingsA.LightColor, InSunLightSettingsB.LightColor, Alpha);
		SunLightSettings.Temperature = FMath::Lerp(InSunLightSettingsA.Temperature, InSunLightSettingsB.Temperature, Alpha);
		SunLightSettings.VolumetricScatteringIntensity = FMath::Lerp(InSunLightSettingsA.VolumetricScatteringIntensity, InSunLightSettingsB.VolumetricScatteringIntensity, Alpha);
		SunLightSettings.AtmosphereDiskColorScale = FMath::Lerp(InSunLightSettingsA.AtmosphereDiskColorScale, InSunLightSettingsB.AtmosphereDiskColorScale, Alpha);
		SunLightSettings.CloudScatteredLuminanceScale = FMath::Lerp(InSunLightSettingsA.CloudScatteredLuminanceScale, InSunLightSettingsB.CloudScatteredLuminanceScale, Alpha);
		SunLightSettings.CloudShadowStrength = FMath::Lerp(InSunLightSettingsA.CloudShadowStrength, InSunLightSettingsB.CloudShadowStrength, Alpha);
		SunLightSettings.CloudShadowOnAtmosphereStrength = FMath::Lerp(InSunLightSettingsA.CloudShadowOnAtmosphereStrength, InSunLightSettingsB.CloudShadowOnAtmosphereStrength, Alpha);
		SunLightSettings.CloudShadowOnSurfaceStrength = FMath::Lerp(InSunLightSettingsA.CloudShadowOnSurfaceStrength, InSunLightSettingsB.CloudShadowOnSurfaceStrength, Alpha);

		SetSunLightSettings(SunLightSettings);
	}
}

void ASkyCreator::LerpMoonLightSettings(FSkyCreatorMoonLightSettings InMoonLightSettingsA, FSkyCreatorMoonLightSettings InMoonLightSettingsB, float Alpha)
{
	if (MoonLight)
	{
		FSkyCreatorMoonLightSettings MoonLightSettings;

		if (!bMoonConstantIntensity) 
		{
			MoonLightSettings.Intensity = FMath::Lerp(InMoonLightSettingsA.Intensity, InMoonLightSettingsB.Intensity, Alpha);
		}
		MoonLightSettings.LightColor = FMath::Lerp(InMoonLightSettingsA.LightColor, InMoonLightSettingsB.LightColor, Alpha);
		MoonLightSettings.Temperature = FMath::Lerp(InMoonLightSettingsA.Temperature, InMoonLightSettingsB.Temperature, Alpha);
		MoonLightSettings.VolumetricScatteringIntensity = FMath::Lerp(InMoonLightSettingsA.VolumetricScatteringIntensity, InMoonLightSettingsB.VolumetricScatteringIntensity, Alpha);
		MoonLightSettings.AtmosphereDiskColorScale = FMath::Lerp(InMoonLightSettingsA.AtmosphereDiskColorScale, InMoonLightSettingsB.AtmosphereDiskColorScale, Alpha);
		MoonLightSettings.CloudScatteredLuminanceScale = FMath::Lerp(InMoonLightSettingsA.CloudScatteredLuminanceScale, InMoonLightSettingsB.CloudScatteredLuminanceScale, Alpha);
		MoonLightSettings.CloudShadowStrength = FMath::Lerp(InMoonLightSettingsA.CloudShadowStrength, InMoonLightSettingsB.CloudShadowStrength, Alpha);
		MoonLightSettings.CloudShadowOnAtmosphereStrength = FMath::Lerp(InMoonLightSettingsA.CloudShadowOnAtmosphereStrength, InMoonLightSettingsB.CloudShadowOnAtmosphereStrength, Alpha);
		MoonLightSettings.CloudShadowOnSurfaceStrength = FMath::Lerp(InMoonLightSettingsA.CloudShadowOnSurfaceStrength, InMoonLightSettingsB.CloudShadowOnSurfaceStrength, Alpha);

		SetMoonLightSettings(MoonLightSettings);
	}
}

void ASkyCreator::LerpExponentialHeightFogSettings(FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettingsA, FSkyCreatorExponentialHeightFogSettings InExponentialHeightFogSettingsB, float Alpha)
{
	if (ExponentialHeightFog)
	{
		FSkyCreatorExponentialHeightFogSettings ExponentialHeightFogSettings;

		ExponentialHeightFogSettings.FogDensity = FMath::Lerp(InExponentialHeightFogSettingsA.FogDensity, InExponentialHeightFogSettingsB.FogDensity, Alpha);
		ExponentialHeightFogSettings.FogHeightFalloff = FMath::Lerp(InExponentialHeightFogSettingsA.FogHeightFalloff, InExponentialHeightFogSettingsB.FogHeightFalloff, Alpha);
		ExponentialHeightFogSettings.FogInscatteringColor = FMath::Lerp(InExponentialHeightFogSettingsA.FogInscatteringColor, InExponentialHeightFogSettingsB.FogInscatteringColor, Alpha);
		ExponentialHeightFogSettings.FogStartDistance = FMath::Lerp(InExponentialHeightFogSettingsA.FogStartDistance, InExponentialHeightFogSettingsB.FogStartDistance, Alpha);

		ExponentialHeightFogSettings.SecondFogDensity = FMath::Lerp(InExponentialHeightFogSettingsA.SecondFogDensity, InExponentialHeightFogSettingsB.SecondFogDensity, Alpha);
		ExponentialHeightFogSettings.SecondFogHeightFalloff = FMath::Lerp(InExponentialHeightFogSettingsA.SecondFogHeightFalloff, InExponentialHeightFogSettingsB.SecondFogHeightFalloff, Alpha);

		ExponentialHeightFogSettings.DirectionalInscatteringExponent = FMath::Lerp(InExponentialHeightFogSettingsA.DirectionalInscatteringExponent, InExponentialHeightFogSettingsB.DirectionalInscatteringExponent, Alpha);
		ExponentialHeightFogSettings.DirectionalInscatteringColor = FMath::Lerp(InExponentialHeightFogSettingsA.DirectionalInscatteringColor, InExponentialHeightFogSettingsB.DirectionalInscatteringColor, Alpha);
		ExponentialHeightFogSettings.DirectionalInscatteringStartDistance = FMath::Lerp(InExponentialHeightFogSettingsA.DirectionalInscatteringStartDistance, InExponentialHeightFogSettingsB.DirectionalInscatteringStartDistance, Alpha);

		ExponentialHeightFogSettings.VolumetricFogScatteringDistribution = FMath::Lerp(InExponentialHeightFogSettingsA.VolumetricFogScatteringDistribution, InExponentialHeightFogSettingsB.VolumetricFogScatteringDistribution, Alpha);
		ExponentialHeightFogSettings.VolumetricFogAlbedo = UKismetMathLibrary::Conv_LinearColorToColor(FMath::Lerp(InExponentialHeightFogSettingsA.VolumetricFogAlbedo, InExponentialHeightFogSettingsB.VolumetricFogAlbedo, Alpha));
		ExponentialHeightFogSettings.VolumetricFogEmissive = FMath::Lerp(InExponentialHeightFogSettingsA.VolumetricFogEmissive, InExponentialHeightFogSettingsB.VolumetricFogEmissive, Alpha);
		ExponentialHeightFogSettings.VolumetricFogExtinctionScale = FMath::Lerp(InExponentialHeightFogSettingsA.VolumetricFogExtinctionScale, InExponentialHeightFogSettingsB.VolumetricFogExtinctionScale, Alpha);

		SetExponentialHeightFogSettings(ExponentialHeightFogSettings);
	}
}

void ASkyCreator::LerpStarMapSettings(FSkyCreatorStarMapSettings InStarMapSettingsA, FSkyCreatorStarMapSettings InStarMapSettingsB, float Alpha)
{
	if (SkySphere)
	{
		FSkyCreatorStarMapSettings StarMapSettings;

		StarMapSettings.StarMapIntensity = FMath::Lerp(InStarMapSettingsA.StarMapIntensity, InStarMapSettingsB.StarMapIntensity, Alpha);
		StarMapSettings.StarMapColorTint = FMath::Lerp(InStarMapSettingsA.StarMapColorTint, InStarMapSettingsB.StarMapColorTint, Alpha);
		StarMapSettings.StarMapTwinkleIntensity = FMath::Lerp(InStarMapSettingsA.StarMapTwinkleIntensity, InStarMapSettingsB.StarMapTwinkleIntensity, Alpha);
		StarMapSettings.StarMapTwinkleSaturation = FMath::Lerp(InStarMapSettingsA.StarMapTwinkleSaturation, InStarMapSettingsB.StarMapTwinkleSaturation, Alpha);
		StarMapSettings.StarMapTwinkleSpeed = FMath::Lerp(InStarMapSettingsA.StarMapTwinkleSpeed, InStarMapSettingsB.StarMapTwinkleSpeed, Alpha);
		StarMapSettings.StarMapHorizonThreshold = FMath::Lerp(InStarMapSettingsA.StarMapHorizonThreshold, InStarMapSettingsB.StarMapHorizonThreshold, Alpha);
		StarMapSettings.StarMapAtmosphereThreshold = FMath::Lerp(InStarMapSettingsA.StarMapAtmosphereThreshold, InStarMapSettingsB.StarMapAtmosphereThreshold, Alpha);
		StarMapSettings.NightZenithColor = FMath::Lerp(InStarMapSettingsA.NightZenithColor, InStarMapSettingsB.NightZenithColor, Alpha);
		StarMapSettings.NightHorizonColor = FMath::Lerp(InStarMapSettingsA.NightHorizonColor, InStarMapSettingsB.NightHorizonColor, Alpha);

		SetStarMapSettings(StarMapSettings);
	}
}

void ASkyCreator::LerpWeatherFXSettings(FSkyCreatorWeatherFXSettings InWeatherFXSettingsA, FSkyCreatorWeatherFXSettings InWeatherFXSettingsB, float Alpha)
{
	if (WeatherFX)
	{
		FSkyCreatorWeatherFXSettings WeatherFXSettings;

		WeatherFXSettings.RainAmount = FMath::Lerp(InWeatherFXSettingsA.RainAmount, InWeatherFXSettingsB.RainAmount, Alpha);
		WeatherFXSettings.RainColor = FMath::Lerp(InWeatherFXSettingsA.RainColor, InWeatherFXSettingsB.RainColor, Alpha);
		WeatherFXSettings.RainGravity = FMath::Lerp(InWeatherFXSettingsA.RainGravity, InWeatherFXSettingsB.RainGravity, Alpha);
		WeatherFXSettings.RainLifetimeMin = FMath::Lerp(InWeatherFXSettingsA.RainLifetimeMin, InWeatherFXSettingsB.RainLifetimeMin, Alpha);
		WeatherFXSettings.RainLifetimeMax = FMath::Lerp(InWeatherFXSettingsA.RainLifetimeMax, InWeatherFXSettingsB.RainLifetimeMax, Alpha);
		WeatherFXSettings.RainSizeMin = FMath::Lerp(InWeatherFXSettingsA.RainSizeMin, InWeatherFXSettingsB.RainSizeMin, Alpha);
		WeatherFXSettings.RainSizeMax = FMath::Lerp(InWeatherFXSettingsA.RainSizeMax, InWeatherFXSettingsB.RainSizeMax, Alpha);

		WeatherFXSettings.RainSplashSizeMin = FMath::Lerp(InWeatherFXSettingsA.RainSplashSizeMin, InWeatherFXSettingsB.RainSplashSizeMin, Alpha);
		WeatherFXSettings.RainSplashSizeMax = FMath::Lerp(InWeatherFXSettingsA.RainSplashSizeMax, InWeatherFXSettingsB.RainSplashSizeMax, Alpha);

		WeatherFXSettings.SnowAmount = FMath::Lerp(InWeatherFXSettingsA.SnowAmount, InWeatherFXSettingsB.SnowAmount, Alpha);
		WeatherFXSettings.SnowColor = FMath::Lerp(InWeatherFXSettingsA.SnowColor, InWeatherFXSettingsB.SnowColor, Alpha);
		WeatherFXSettings.SnowGravity = FMath::Lerp(InWeatherFXSettingsA.SnowGravity, InWeatherFXSettingsB.SnowGravity, Alpha);
		WeatherFXSettings.SnowTurbulence = FMath::Lerp(InWeatherFXSettingsA.SnowTurbulence, InWeatherFXSettingsB.SnowTurbulence, Alpha);
		WeatherFXSettings.SnowLifetimeMin = FMath::Lerp(InWeatherFXSettingsA.SnowLifetimeMin, InWeatherFXSettingsB.SnowLifetimeMin, Alpha);
		WeatherFXSettings.SnowLifetimeMax = FMath::Lerp(InWeatherFXSettingsA.SnowLifetimeMax, InWeatherFXSettingsB.SnowLifetimeMax, Alpha);
		WeatherFXSettings.SnowSizeMin = FMath::Lerp(InWeatherFXSettingsA.SnowSizeMin, InWeatherFXSettingsB.SnowSizeMin, Alpha);
		WeatherFXSettings.SnowSizeMax = FMath::Lerp(InWeatherFXSettingsA.SnowSizeMax, InWeatherFXSettingsB.SnowSizeMax, Alpha);

		// need to redo this part later on
		if (InWeatherFXSettingsA.EnableLightnings == true && InWeatherFXSettingsB.EnableLightnings == false && Alpha <= 0.25f)
		{
			WeatherFXSettings.EnableLightnings = true;
		}
		else if (InWeatherFXSettingsA.EnableLightnings == false && InWeatherFXSettingsB.EnableLightnings == true && Alpha >= 0.75f)
		{
			WeatherFXSettings.EnableLightnings = true;
		}
		else if (InWeatherFXSettingsA.EnableLightnings == false && InWeatherFXSettingsB.EnableLightnings == false)
		{
			WeatherFXSettings.EnableLightnings = false;
		}
		else if (InWeatherFXSettingsA.EnableLightnings == true && InWeatherFXSettingsB.EnableLightnings == true)
		{
			WeatherFXSettings.EnableLightnings = true;
		}
		WeatherFXSettings.LightningSpawnIntervalMin = FMath::Lerp(InWeatherFXSettingsA.LightningSpawnIntervalMin, InWeatherFXSettingsB.LightningSpawnIntervalMin, Alpha);
		WeatherFXSettings.LightningSpawnIntervalMax = FMath::Lerp(InWeatherFXSettingsA.LightningSpawnIntervalMax, InWeatherFXSettingsB.LightningSpawnIntervalMax, Alpha);
		WeatherFXSettings.LightningColorMin = FMath::Lerp(InWeatherFXSettingsA.LightningColorMin, InWeatherFXSettingsB.LightningColorMin, Alpha);
		WeatherFXSettings.LightningColorMax = FMath::Lerp(InWeatherFXSettingsA.LightningColorMax, InWeatherFXSettingsB.LightningColorMax, Alpha);
		WeatherFXSettings.LightningLifetime = FMath::Lerp(InWeatherFXSettingsA.LightningLifetime, InWeatherFXSettingsB.LightningLifetime, Alpha);

		WeatherFXSettings.LightningBoltSpawnChance = FMath::Lerp(InWeatherFXSettingsA.LightningBoltSpawnChance, InWeatherFXSettingsB.LightningBoltSpawnChance, Alpha);
		WeatherFXSettings.LightningBoltWidthMin = FMath::Lerp(InWeatherFXSettingsA.LightningBoltWidthMin, InWeatherFXSettingsB.LightningBoltWidthMin, Alpha);
		WeatherFXSettings.LightningBoltWidthMax = FMath::Lerp(InWeatherFXSettingsA.LightningBoltWidthMax, InWeatherFXSettingsB.LightningBoltWidthMax, Alpha);
		WeatherFXSettings.LightningBoltLengthMin = FMath::Lerp(InWeatherFXSettingsA.LightningBoltLengthMin, InWeatherFXSettingsB.LightningBoltLengthMin, Alpha);
		WeatherFXSettings.LightningBoltLengthMax = FMath::Lerp(InWeatherFXSettingsA.LightningBoltLengthMax, InWeatherFXSettingsB.LightningBoltLengthMax, Alpha);
		WeatherFXSettings.LightningBoltCurveFrequency = FMath::Lerp(InWeatherFXSettingsA.LightningBoltCurveFrequency, InWeatherFXSettingsB.LightningBoltCurveFrequency, Alpha);
		WeatherFXSettings.LightningBoltCurveStrengthMin = FMath::Lerp(InWeatherFXSettingsA.LightningBoltCurveStrengthMin, InWeatherFXSettingsB.LightningBoltCurveStrengthMin, Alpha);
		WeatherFXSettings.LightningBoltCurveStrengthMax = FMath::Lerp(InWeatherFXSettingsA.LightningBoltCurveStrengthMax, InWeatherFXSettingsB.LightningBoltCurveStrengthMax, Alpha);
		WeatherFXSettings.LightningBoltJitterStrength = FMath::Lerp(InWeatherFXSettingsA.LightningBoltJitterStrength, InWeatherFXSettingsB.LightningBoltJitterStrength, Alpha);

		WeatherFXSettings.LightningBranchSpawnChance = FMath::Lerp(InWeatherFXSettingsA.LightningBranchSpawnChance, InWeatherFXSettingsB.LightningBranchSpawnChance, Alpha);
		WeatherFXSettings.LightningBranchScaleMin = FMath::Lerp(InWeatherFXSettingsA.LightningBranchScaleMin, InWeatherFXSettingsB.LightningBranchScaleMin, Alpha);
		WeatherFXSettings.LightningBranchScaleMax = FMath::Lerp(InWeatherFXSettingsA.LightningBranchScaleMax, InWeatherFXSettingsB.LightningBranchScaleMax, Alpha);

		WeatherFXSettings.RainbowAmount = FMath::Lerp(InWeatherFXSettingsA.RainbowAmount, InWeatherFXSettingsB.RainbowAmount, Alpha);
		WeatherFXSettings.RainbowColor = FMath::Lerp(InWeatherFXSettingsA.RainbowColor, InWeatherFXSettingsB.RainbowColor, Alpha);
		WeatherFXSettings.RainbowRadius = FMath::Lerp(InWeatherFXSettingsA.RainbowRadius, InWeatherFXSettingsB.RainbowRadius, Alpha);
		WeatherFXSettings.RainbowThickness = FMath::Lerp(InWeatherFXSettingsA.RainbowThickness, InWeatherFXSettingsB.RainbowThickness, Alpha);
		WeatherFXSettings.SecondaryRainbowColor = FMath::Lerp(InWeatherFXSettingsA.SecondaryRainbowColor, InWeatherFXSettingsB.SecondaryRainbowColor, Alpha);
		WeatherFXSettings.SecondaryRainbowRadius = FMath::Lerp(InWeatherFXSettingsA.SecondaryRainbowRadius, InWeatherFXSettingsB.SecondaryRainbowRadius, Alpha);
		WeatherFXSettings.SecondaryRainbowThickness = FMath::Lerp(InWeatherFXSettingsA.SecondaryRainbowThickness, InWeatherFXSettingsB.SecondaryRainbowThickness, Alpha);

		SetWeatherFXSettings(WeatherFXSettings);
	}
}

void ASkyCreator::LerpWeatherMaterialFXSettings(FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettingsA, FSkyCreatorWeatherMaterialFXSettings InWeatherMaterialFXSettingsB, float Alpha)
{
	if (CommonMPC)
	{
		FSkyCreatorWeatherMaterialFXSettings WeatherMaterialFXSettings;

		WeatherMaterialFXSettings.WetnessAmount = FMath::Lerp(InWeatherMaterialFXSettingsA.WetnessAmount, InWeatherMaterialFXSettingsB.WetnessAmount, Alpha);
		WeatherMaterialFXSettings.WetnessColor = FMath::Lerp(InWeatherMaterialFXSettingsA.WetnessColor, InWeatherMaterialFXSettingsB.WetnessColor, Alpha);

		WeatherMaterialFXSettings.PuddlesAmount = FMath::Lerp(InWeatherMaterialFXSettingsA.PuddlesAmount, InWeatherMaterialFXSettingsB.PuddlesAmount, Alpha);
		WeatherMaterialFXSettings.PuddlesColor = FMath::Lerp(InWeatherMaterialFXSettingsA.PuddlesColor, InWeatherMaterialFXSettingsB.PuddlesColor, Alpha);

		WeatherMaterialFXSettings.WindRipplesIntensity = FMath::Lerp(InWeatherMaterialFXSettingsA.WindRipplesIntensity, InWeatherMaterialFXSettingsB.WindRipplesIntensity, Alpha);
		WeatherMaterialFXSettings.WindRipplesSpeed = FMath::Lerp(InWeatherMaterialFXSettingsA.WindRipplesSpeed, InWeatherMaterialFXSettingsB.WindRipplesSpeed, Alpha);

		WeatherMaterialFXSettings.RainRipplesAmount = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesAmount, InWeatherMaterialFXSettingsB.RainRipplesAmount, Alpha);
		WeatherMaterialFXSettings.RainRipplesIntensity = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesIntensity, InWeatherMaterialFXSettingsB.RainRipplesIntensity, Alpha);
		WeatherMaterialFXSettings.RainRipplesForceMinRadius = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesForceMinRadius, InWeatherMaterialFXSettingsB.RainRipplesForceMinRadius, Alpha);
		WeatherMaterialFXSettings.RainRipplesForceMaxRadius = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesForceMaxRadius, InWeatherMaterialFXSettingsB.RainRipplesForceMaxRadius, Alpha);
		WeatherMaterialFXSettings.RainRipplesRingsNumber = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesRingsNumber, InWeatherMaterialFXSettingsB.RainRipplesRingsNumber, Alpha);
		WeatherMaterialFXSettings.RainRipplesRingsDelay = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesRingsDelay, InWeatherMaterialFXSettingsB.RainRipplesRingsDelay, Alpha);
		WeatherMaterialFXSettings.RainRipplesDamping = FMath::Lerp(InWeatherMaterialFXSettingsA.RainRipplesDamping, InWeatherMaterialFXSettingsB.RainRipplesDamping, Alpha);
		WeatherMaterialFXSettings.WindRipplesIntensity = FMath::Lerp(InWeatherMaterialFXSettingsA.WindRipplesIntensity, InWeatherMaterialFXSettingsB.WindRipplesIntensity, Alpha);
		WeatherMaterialFXSettings.WindRipplesSpeed = FMath::Lerp(InWeatherMaterialFXSettingsA.WindRipplesSpeed, InWeatherMaterialFXSettingsB.WindRipplesSpeed, Alpha);

		WeatherMaterialFXSettings.SnowAmount = FMath::Lerp(InWeatherMaterialFXSettingsA.SnowAmount, InWeatherMaterialFXSettingsB.SnowAmount, Alpha);
		WeatherMaterialFXSettings.SnowColor = FMath::Lerp(InWeatherMaterialFXSettingsA.SnowColor, InWeatherMaterialFXSettingsB.SnowColor, Alpha);
		WeatherMaterialFXSettings.SnowIntensity = FMath::Lerp(InWeatherMaterialFXSettingsA.SnowIntensity, InWeatherMaterialFXSettingsB.SnowIntensity, Alpha);
		WeatherMaterialFXSettings.SnowSparklesThreshold = FMath::Lerp(InWeatherMaterialFXSettingsA.SnowSparklesThreshold, InWeatherMaterialFXSettingsB.SnowSparklesThreshold, Alpha);

		SetWeatherMaterialFXSettings(WeatherMaterialFXSettings);
	}
}

void ASkyCreator::LerpWindSettings(FSkyCreatorWindSettings InWindSettingsA, FSkyCreatorWindSettings InWindSettingsB, float Alpha)
{
	if (VolumetricCloud && VolumetricCloudMID && WeatherFX)
	{
		FSkyCreatorWindSettings WindSettings;

		WindSettings.CloudWindDirection = FMath::Lerp(InWindSettingsA.CloudWindDirection, InWindSettingsB.CloudWindDirection, Alpha);
		WindSettings.CloudWindSpeed = FMath::Lerp(InWindSettingsA.CloudWindSpeed, InWindSettingsB.CloudWindSpeed, Alpha);
		//WindSettings.HighCloudWindDirection = FMath::Lerp(InWindSettingsA.HighCloudWindDirection, InWindSettingsB.HighCloudWindDirection, Alpha);
		//WindSettings.HighCloudWindSpeed = FMath::Lerp(InWindSettingsA.HighCloudWindSpeed, InWindSettingsB.HighCloudWindSpeed, Alpha);
		WindSettings.CloudNoiseShapeWindDirection = FMath::Lerp(InWindSettingsA.CloudNoiseShapeWindDirection, InWindSettingsB.CloudNoiseShapeWindDirection, Alpha);
		WindSettings.CloudNoiseShapeWindSpeedHorizontal = FMath::Lerp(InWindSettingsA.CloudNoiseShapeWindSpeedHorizontal, InWindSettingsB.CloudNoiseShapeWindSpeedHorizontal, Alpha);
		WindSettings.CloudNoiseDetailWindSpeedVertical = FMath::Lerp(InWindSettingsA.CloudNoiseDetailWindSpeedVertical, InWindSettingsB.CloudNoiseDetailWindSpeedVertical, Alpha);

		WindSettings.PrecipitationWindDirection = FMath::Lerp(InWindSettingsA.PrecipitationWindDirection, InWindSettingsB.PrecipitationWindDirection, Alpha);
		WindSettings.PrecipitationWindSpeed = FMath::Lerp(InWindSettingsA.PrecipitationWindSpeed, InWindSettingsB.PrecipitationWindSpeed, Alpha);

		if (!bIndependentWindControl)
		{
			SetWindSettings(WindSettings);
		}
	}
}

void ASkyCreator::LerpWindIndependentSettings(FSkyCreatorWindSettings InWindSettingsA, FSkyCreatorWindSettings InWindSettingsB, float Alpha)
{
	if (VolumetricCloud && VolumetricCloudMID && WeatherFX)
	{
		FSkyCreatorWindSettings WindSettings;

		WindSettings.CloudWindDirection = FMath::Lerp(InWindSettingsA.CloudWindDirection, InWindSettingsB.CloudWindDirection, Alpha);
		WindSettings.CloudWindSpeed = FMath::Lerp(InWindSettingsA.CloudWindSpeed, InWindSettingsB.CloudWindSpeed, Alpha);
		//WindSettings.HighCloudWindDirection = FMath::Lerp(InWindSettingsA.HighCloudWindDirection, InWindSettingsB.HighCloudWindDirection, Alpha);
		//WindSettings.HighCloudWindSpeed = FMath::Lerp(InWindSettingsA.HighCloudWindSpeed, InWindSettingsB.HighCloudWindSpeed, Alpha);
		WindSettings.CloudNoiseShapeWindDirection = FMath::Lerp(InWindSettingsA.CloudNoiseShapeWindDirection, InWindSettingsB.CloudNoiseShapeWindDirection, Alpha);
		WindSettings.CloudNoiseShapeWindSpeedHorizontal = FMath::Lerp(InWindSettingsA.CloudNoiseShapeWindSpeedHorizontal, InWindSettingsB.CloudNoiseShapeWindSpeedHorizontal, Alpha);
		WindSettings.CloudNoiseDetailWindSpeedVertical = FMath::Lerp(InWindSettingsA.CloudNoiseDetailWindSpeedVertical, InWindSettingsB.CloudNoiseDetailWindSpeedVertical, Alpha);

		WindSettings.PrecipitationWindDirection = FMath::Lerp(InWindSettingsA.PrecipitationWindDirection, InWindSettingsB.PrecipitationWindDirection, Alpha);
		WindSettings.PrecipitationWindSpeed = FMath::Lerp(InWindSettingsA.PrecipitationWindSpeed, InWindSettingsB.PrecipitationWindSpeed, Alpha);

		if (bIndependentWindControl)
		{
			SetWindIndependentSettings(WindSettings);
		}
	}
}

void ASkyCreator::LerpPostProcessSettings(FSkyCreatorPostProcessSettings InPostProcessSettingsA, FSkyCreatorPostProcessSettings InPostProcessSettingsB, float Alpha)
{
	if (PostProcess)
	{
		FSkyCreatorPostProcessSettings PostProcessSettings;

		PostProcessSettings.BloomIntensity = FMath::Lerp(InPostProcessSettingsA.BloomIntensity, InPostProcessSettingsB.BloomIntensity, Alpha);
		PostProcessSettings.BloomThreshold = FMath::Lerp(InPostProcessSettingsA.BloomThreshold, InPostProcessSettingsB.BloomThreshold, Alpha);
		PostProcessSettings.ExposureCompensation = FMath::Lerp(InPostProcessSettingsA.ExposureCompensation, InPostProcessSettingsB.ExposureCompensation, Alpha);

		SetPostProcessSettings(PostProcessSettings);
	}
}

void ASkyCreator::LerpWeatherSettings(FSkyCreatorWeatherSettings InWeatherSettingsA, FSkyCreatorWeatherSettings InWeatherSettingsB, float Alpha)
{
	LerpSkyAtmosphereSettings(InWeatherSettingsA.SkyAtmosphereSettings, InWeatherSettingsB.SkyAtmosphereSettings, Alpha);
	LerpVolumetricCloudSettings(InWeatherSettingsA.VolumetricCloudSettings, InWeatherSettingsB.VolumetricCloudSettings, Alpha);
	LerpBackgroundCloudSettings(InWeatherSettingsA.BackgroundCloudSettings, InWeatherSettingsB.BackgroundCloudSettings, Alpha);
	LerpSkyLightSettings(InWeatherSettingsA.SkyLightSettings, InWeatherSettingsB.SkyLightSettings, Alpha);
	LerpSunLightSettings(InWeatherSettingsA.SunLightSettings, InWeatherSettingsB.SunLightSettings, Alpha);
	LerpMoonLightSettings(InWeatherSettingsA.MoonLightSettings, InWeatherSettingsB.MoonLightSettings, Alpha);
	LerpExponentialHeightFogSettings(InWeatherSettingsA.ExponentialHeightFogSettings, InWeatherSettingsB.ExponentialHeightFogSettings, Alpha);
	LerpStarMapSettings(InWeatherSettingsA.StarMapSettings, InWeatherSettingsB.StarMapSettings, Alpha);
	LerpWeatherFXSettings(InWeatherSettingsA.WeatherFXSettings, InWeatherSettingsB.WeatherFXSettings, Alpha);
	LerpWeatherMaterialFXSettings(InWeatherSettingsA.WeatherMaterialFXSettings, InWeatherSettingsB.WeatherMaterialFXSettings, Alpha);
	LerpWindSettings(InWeatherSettingsA.WindSettings, InWeatherSettingsB.WindSettings, Alpha);
	LerpPostProcessSettings(InWeatherSettingsA.PostProcessSettings, InWeatherSettingsB.PostProcessSettings, Alpha);
}

void ASkyCreator::LerpWetnessAmount(float WetnessAmountA, float WetnessAmountB, float Alpha)
{
	if (CommonMPC)
	{
		WeatherSettings.WeatherMaterialFXSettings.WetnessAmount = FMath::Lerp(WetnessAmountA, WetnessAmountB, Alpha);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Wetness Amount", WeatherSettings.WeatherMaterialFXSettings.WetnessAmount);
	}
}

void ASkyCreator::LerpPuddlesAmount(float PuddlesAmountA, float PuddlesAmountB, float Alpha)
{
	if (CommonMPC)
	{
		WeatherSettings.WeatherMaterialFXSettings.PuddlesAmount = FMath::Lerp(PuddlesAmountA, PuddlesAmountB, Alpha);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Puddles Amount", WeatherSettings.WeatherMaterialFXSettings.PuddlesAmount);
	}
}

void ASkyCreator::LerpSnowAmount(float SnowAmountA, float SnowAmountB, float Alpha)
{
	if (CommonMPC)
	{
		WeatherSettings.WeatherMaterialFXSettings.SnowAmount = FMath::Lerp(SnowAmountA, SnowAmountB, Alpha);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Snow Amount", WeatherSettings.WeatherMaterialFXSettings.SnowAmount);
	}
}

FRotator ASkyCreator::GetSunPosition(float Time)
{
	FRotator Result;

	SunDawnTime = SunriseTime - SunDawnOffsetTime;
	SunDuskTime = SunsetTime + SunDuskOffsetTime;

	if (SunPositionType == SunPositionType_Simple)
	{
		const float BeforeDawn = FMath::Lerp(90.0f, SunMinAngleAtDawnDusk, FMath::GetMappedRangeValueClamped(FVector2D(0, SunDawnTime), FVector2D(0, 1), Time));
		const float BeforeSunset = FMath::Lerp(SunMinAngleAtDawnDusk, 0.0f, FMath::GetMappedRangeValueClamped(FVector2D(SunDawnTime, SunriseTime), FVector2D(0, 1), Time));
		const float Midday = FMath::Lerp(0.0f, -180.0f, FMath::GetMappedRangeValueClamped(FVector2D(SunriseTime, SunsetTime), FVector2D(0, 1), Time));
		const float AfterSunset = FMath::Lerp(-180.0f, -180.0f - SunMinAngleAtDawnDusk, FMath::GetMappedRangeValueClamped(FVector2D(SunsetTime, SunDuskTime), FVector2D(0, 1), Time));
		const float AfterDusk = FMath::Lerp(-180.0f - SunMinAngleAtDawnDusk, -270.0f, FMath::GetMappedRangeValueClamped(FVector2D(SunDuskTime, 24), FVector2D(0, 1), Time));

		const bool bBeforeDawn = UKismetMathLibrary::InRange_FloatFloat(Time, 0, SunDawnTime, true, true);
		const bool bBeforeSunrise = UKismetMathLibrary::InRange_FloatFloat(Time, 0, SunriseTime, true, true);
		const bool bMidday = UKismetMathLibrary::InRange_FloatFloat(Time, SunriseTime, SunsetTime, true, true);
		const bool bAfterDusk = UKismetMathLibrary::InRange_FloatFloat(Time, SunDuskTime, 24, true, true);

		const float SunAngle = UKismetMathLibrary::SelectFloat(Midday, UKismetMathLibrary::SelectFloat(UKismetMathLibrary::SelectFloat(BeforeDawn, BeforeSunset, bBeforeDawn), UKismetMathLibrary::SelectFloat(AfterDusk, AfterSunset, bAfterDusk), bBeforeSunrise), bMidday);

		Result = UKismetMathLibrary::ComposeRotators(FRotator(SunAngle, 0, 0), FRotator(0, SunAzimuth, SunElevation - 90.0f));
	}
	else
	{
		SunPositionData = USkyCreatorFunctionLibrary::GetRealSunPosition(Latitude, Longitude, TimeZone, bDaylightSavingTime, Date);
		Result = FRotator(SunPositionData.Elevation + 180.0, SunPositionData.Azimuth, 0);
	}

//	else if (SunOrbitType == OrbitType_FixedElevation) {
//		Result = UKismetMathLibrary::ComposeRotators(FRotator(SunElevation * -1.0f, SunAzimuth, 0), FRotator(0, FMath::Lerp(360.0f, 0.0f, FMath::GetMappedRangeValueClamped(FVector2D(0, 24.0f), FVector2D(0, 1.0f), Time)), 0));
//	}

	return Result;
}

FRotator ASkyCreator::GetMoonPosition(float Time)
{
	FRotator Result;

	if (MoonPositionType == MoonPositionType_Simple)
	{
		const float Midnight = FMath::Lerp(-180.0f, -360.0f, FMath::GetMappedRangeValueClamped(FVector2D(SunriseTime, SunsetTime), FVector2D(0, 1), Time));
		const float AfterMoonrise = FMath::Lerp(0.0f, -90.0f, FMath::GetMappedRangeValueClamped(FVector2D(MoonriseTime, 24), FVector2D(0, 1), Time));
		const float BeforeMoonset = FMath::Lerp(-90.0f, -180.0f, FMath::GetMappedRangeValueClamped(FVector2D(0, MoonsetTime), FVector2D(0, 1), Time));

		const bool bMidnight = UKismetMathLibrary::InRange_FloatFloat(Time, MoonsetTime, MoonriseTime, true, true);
		const bool bAfterMoonrise = UKismetMathLibrary::InRange_FloatFloat(Time, MoonriseTime, 24.0f, true, true);

		const float MoonAngle = UKismetMathLibrary::SelectFloat(Midnight, UKismetMathLibrary::SelectFloat(AfterMoonrise, BeforeMoonset, bAfterMoonrise), bMidnight);

		Result = UKismetMathLibrary::ComposeRotators(FRotator(MoonAngle, 0, 0), FRotator(0, MoonAzimuth, MoonElevation - 90.0f));
	}
	else if (MoonPositionType == MoonPositionType_SimpleFixedElevation) 
	{
		Result = UKismetMathLibrary::ComposeRotators(FRotator(MoonElevation * -1.0f, MoonAzimuth, 0), FRotator(0, FMath::Lerp(360.0f, 0.0f, FMath::GetMappedRangeValueClamped(FVector2D(0, 24.0f), FVector2D(0, 1.0f), Time)), 0));
	}
	else 
	{
		MoonPositionData = USkyCreatorFunctionLibrary::GetRealMoonPosition(Latitude, Longitude, TimeZone, bDaylightSavingTime, Date);
		Result = FRotator(MoonPositionData.Elevation + 180.0, MoonPositionData.Azimuth, 0);
	}

	return Result;
}

FRotator ASkyCreator::GetStarMapRotation() const
{
	FRotator Result;

	switch (StarMapRotationType)
	{
		case 0: // No Rotation
		{
			Result = FRotator(0, 0, 0);
			break;
		}
		case 1: // Follow Sun
		{
			Result = SunLight->GetRelativeRotation();
			break;
		}
		case 2: // Follow Moon
		{
			Result = MoonLight->GetRelativeRotation();
			break;
		}
		default: // No Rotation
		{
			Result = FRotator(0, 0, 0);
			break;
		}
	}

	return Result;
}

void ASkyCreator::SetSunMoonPosition(float InTime)
{
	if (bControlSunDirection)
	{
		SunLight->SetRelativeRotation(GetSunPosition(InTime));
	}
	if (bControlMoonDirection)
	{
		MoonLight->SetRelativeRotation(GetMoonPosition(InTime));
	}

	if (StarMapRotationType != StarMapRotationType_NoRotation) {
//		if (SkySphereMID)
//		{
//			SkySphereMID->SetVectorParameterValue("StarMap_Rotation", UKismetMathLibrary::Conv_VectorToLinearColor(GetStarMapRotation(InTime).Vector())); // Star Map Rotation
//		}
		if (CommonMPC)
		{
			UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Star Map Rotation", FLinearColor(GetStarMapRotation().Vector()));
		}
	}
/*	if (SkySphereMID)
	{
		SkySphereMID->SetVectorParameterValue("Moon_Direction", UKismetMathLibrary::Conv_VectorToLinearColor(MoonLight->GetComponentRotation().Vector() * -1.0f));
	}*/

	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Sun Light Direction", FLinearColor(SunLight->GetComponentRotation().Vector() * -1.0f));
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Moon Light Direction", FLinearColor(MoonLight->GetComponentRotation().Vector() * -1.0f));
	}

	if (WeatherFX)
	{
		WeatherFX->SetNiagaraVariableVec3("Sun Rotation", SunLight->GetComponentRotation().Vector());
	}

	UpdateSunMoonIntensity(TimeOfDay);
}

FSkyCreatorSkyAtmosphereSettings ASkyCreator::GetSkyAtmosphereSettings() const
{
	return WeatherSettings.SkyAtmosphereSettings;
}

FSkyCreatorVolumetricCloudSettings ASkyCreator::GetVolumetricCloudSettings() const
{
	return WeatherSettings.VolumetricCloudSettings;
}

FSkyCreatorBackgroundCloudSettings ASkyCreator::GetBackgroundCloudSettings() const
{
	return WeatherSettings.BackgroundCloudSettings;
}

FSkyCreatorSkyLightSettings ASkyCreator::GetSkyLightSettings() const
{
	return WeatherSettings.SkyLightSettings;
}

FSkyCreatorSunLightSettings ASkyCreator::GetSunLightSettings() const
{
	return WeatherSettings.SunLightSettings;
}

FSkyCreatorMoonLightSettings ASkyCreator::GetMoonLightSettings() const
{
	return WeatherSettings.MoonLightSettings;
}

FSkyCreatorExponentialHeightFogSettings ASkyCreator::GetExponentialHeightFogSettings() const
{
	return WeatherSettings.ExponentialHeightFogSettings;
}

FSkyCreatorWeatherFXSettings ASkyCreator::GetWeatherFXSettings() const
{
	return WeatherSettings.WeatherFXSettings;
}

FSkyCreatorWeatherMaterialFXSettings ASkyCreator::GetWeatherMaterialFXSettings() const
{
	return WeatherSettings.WeatherMaterialFXSettings;
}

FSkyCreatorStarMapSettings ASkyCreator::GetStarMapSettings() const
{
	return WeatherSettings.StarMapSettings;
}

FSkyCreatorWindSettings ASkyCreator::GetWindSettings() const
{
	return WeatherSettings.WindSettings;
}

FSkyCreatorPostProcessSettings ASkyCreator::GetPostProcessSettings() const
{
	return WeatherSettings.PostProcessSettings;
}

FSkyCreatorWeatherSettings ASkyCreator::GetWeatherSettings() const
{
	return WeatherSettings;
}

void ASkyCreator::SetTime(float InTime)
{
	InTime = FGenericPlatformMath::Fmod(InTime, 24.0f); // Internal time stays at [0, 24] range
	TimeOfDay = InTime;
	Date = UKismetMathLibrary::MakeDateTime(Year, Month, Day, TimeOfDay, UKismetMathLibrary::Fraction(TimeOfDay) * 60, UKismetMathLibrary::Fraction(UKismetMathLibrary::Fraction(TimeOfDay) * 60) * 60, 0); //UKismetMathLibrary::Fraction(TimeOfDay) * 60

//	FDateTime CalcTime(Year, Month, Day, TimeOfDay);

//	Date = CalcTime;

	SetSunMoonPosition(TimeOfDay);
}

void ASkyCreator::RealtimeTimeOfDay(float DeltaSeconds, float DayCycleDuration)
{
	TimeOfDay = ((DeltaSeconds / (DayCycleDuration)) * 24.0f) + TimeOfDay;
	SetTime(TimeOfDay);
}

float ASkyCreator::GetTime() const
{
	return TimeOfDay;
}
/*
void ASkyCreator::SetMoonPhase(float InMoonPhase)
{
	MoonPhase = InMoonPhase;
//	SkySphereMID->SetScalarParameterValue("Moon_Phase", MoonPhase);
	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Phase", MoonPhase);
	}
}
*/
float ASkyCreator::GetMoonPhase() const
{
	return MoonPhase;
}

void ASkyCreator::UpdateSunMoonIntensity(float InTime)
{
	SunDawnTime = SunriseTime - SunDawnOffsetTime;
	SunDuskTime = SunsetTime + SunDuskOffsetTime;

	// Optimized SunLight & MoonLight intensity in such a way that they will never illuminate at the same time

	// Old method
	/*
	if (bLightOptimization) {
		if (TimeOfDay < SunriseTime) {
			SunLight->SetIntensity(FMath::Lerp(0.0f, WeatherSettings.SunLightSettings.Intensity, FMath::GetMappedRangeValueClamped(FVector2D(SunDawnTime, SunDawnTime + SunFadeInOutTime), FVector2D(0, 1), TimeOfDay)));
			MoonLight->SetIntensity(FMath::Lerp(0.0f, WeatherSettings.MoonLightSettings.Intensity, FMath::GetMappedRangeValueClamped(FVector2D(SunDawnTime - MoonFadeInOutTime, SunDawnTime), FVector2D(1, 0), TimeOfDay)));
		}
		else if (TimeOfDay > SunsetTime) {
			SunLight->SetIntensity(FMath::Lerp(0.0f, WeatherSettings.SunLightSettings.Intensity, FMath::GetMappedRangeValueClamped(FVector2D(SunDuskTime - SunFadeInOutTime, SunDuskTime), FVector2D(1, 0), TimeOfDay)));
			MoonLight->SetIntensity(FMath::Lerp(0.0f, WeatherSettings.MoonLightSettings.Intensity, FMath::GetMappedRangeValueClamped(FVector2D(SunDuskTime, SunDuskTime + MoonFadeInOutTime), FVector2D(0, 1), TimeOfDay)));
		}
		else if (TimeOfDay > SunriseTime && TimeOfDay < SunsetTime) {
			SunLight->SetIntensity(WeatherSettings.SunLightSettings.Intensity);
			MoonLight->SetIntensity(0.0f);
		}
		else {
			MoonLight->SetIntensity(0.0f);
		}
	}
	else {
		SunLight->SetIntensity(WeatherSettings.SunLightSettings.Intensity);
		MoonLight->SetIntensity(WeatherSettings.MoonLightSettings.Intensity);
	}*/
	
	if (bMoonPhaseLightIntensityScale)
	{
		float MoonLightIntensity = 0.0f;
		
		if (bMoonConstantIntensity)
		{
			MoonLightIntensity = FMath::InterpSinInOut(MoonIntensity * MoonPhaseLightIntensityMaxScale, MoonIntensity * MoonPhaseLightIntensityMinScale, MoonPhase * 2);
		}
		else
		{
			MoonLightIntensity = FMath::InterpSinInOut(WeatherSettings.MoonLightSettings.Intensity * MoonPhaseLightIntensityMaxScale, WeatherSettings.MoonLightSettings.Intensity * MoonPhaseLightIntensityMinScale, MoonPhase * 2);
		}
		MoonLight->SetIntensity(MoonLightIntensity);
	}

	SunCurrentElevation = SunLight->GetRelativeRotation().Pitch;

	if (bLightTransition)
	{
		if (UKismetMathLibrary::InRange_FloatFloat(SunCurrentElevation, TransitionStartSunAngle, TransitionEndSunAngle))
		{
			SunSurfaceBrightness = FMath::GetMappedRangeValueClamped(FVector2D(TransitionStartSunAngle, TransitionMiddleSunAngle), FVector2D(1, 0), SunCurrentElevation);
//			SunCurrentIntensity = FMath::Lerp(0.0f, SunIntensity, SunSurfaceBrightness);
			MoonSurfaceBrightness = FMath::GetMappedRangeValueClamped(FVector2D(TransitionMiddleSunAngle, TransitionEndSunAngle), FVector2D(0, 1), SunCurrentElevation);
//			MoonCurrentIntensity = FMath::Lerp(0.0f, MoonIntensity, MoonSurfaceBrightness);
		}
		else if (SunCurrentElevation < TransitionStartSunAngle)
		{
			SunSurfaceBrightness = 1.0f;
			MoonSurfaceBrightness = 0.0f;
		}
		else if (SunCurrentElevation > TransitionEndSunAngle)
		{
			SunSurfaceBrightness = 0.0f;
			MoonSurfaceBrightness = 1.0f;
		}

		// Sun is always dominant
		//if (SunLight->DisabledBrightness == 0) {
		if (SunSurfaceBrightness == 0)
		{
			SunLight->SetCastShadows(false);
			SunLight->bCastCloudShadows = false;
			SunLight->SetCastVolumetricShadow(false);
			//SunLight->SetDynamicShadowCascades(0);

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
			SunLight->ForwardShadingPriority = 0;
#endif
#endif

			MoonLight->SetCastShadows(true);
			MoonLight->bCastCloudShadows = true;
			MoonLight->SetCastVolumetricShadow(true);
			//MoonLight->SetDynamicShadowCascades(3);

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
			MoonLight->ForwardShadingPriority = 1;
#endif
#endif
		}
		else
		{
			SunLight->SetCastShadows(true);
			SunLight->bCastCloudShadows = true;
			SunLight->SetCastVolumetricShadow(true);
			//SunLight->SetDynamicShadowCascades(3);

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
			SunLight->ForwardShadingPriority = 1;
#endif
#endif

			MoonLight->SetCastShadows(false);
			MoonLight->bCastCloudShadows = false;
			MoonLight->SetCastVolumetricShadow(false);
			//MoonLight->SetDynamicShadowCascades(0);

#if ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION >= 1
			MoonLight->ForwardShadingPriority = 0;
#endif
#endif
		}
//		SunLight->SetLightBrightness(SunBrightness);
//		MoonLight->SetLightBrightness(MoonBrightness);
	}
	else
	{
		SunLight->SetCastShadows(true);
		SunLight->bCastCloudShadows = true;
		SunLight->SetCastVolumetricShadow(true);
		SunSurfaceBrightness = 1.0f;

		MoonLight->SetCastShadows(true);
		MoonLight->bCastCloudShadows = true;
		MoonLight->SetCastVolumetricShadow(true);
		MoonSurfaceBrightness = 1.0f;
	}
//	SunLight->SetIntensity(WeatherSettings.SunLightSettings.Intensity);
//	MoonLight->SetIntensity(WeatherSettings.MoonLightSettings.Intensity);

	SunLight->SetLightFunctionDisabledBrightness(SunSurfaceBrightness);
	MoonLight->SetLightFunctionDisabledBrightness(MoonSurfaceBrightness);

	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Sun Surface Brightness", SunSurfaceBrightness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Surface Brightness", MoonSurfaceBrightness);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Current Sun Elevation", SunCurrentElevation);
	}

	SunLight->MarkRenderStateDirty();
	MoonLight->MarkRenderStateDirty();
	ExponentialHeightFog->MarkRenderStateDirty();
}

void ASkyCreator::UpdateSettings()
{
	SetComponentsSettings();

	CaptureOcclusion(CameraLocationSnapped);

	if (bUseEditorTimeOfDay)
	{
		SetTime(EditorTimeOfDay);
	}

	if (bUseEditorWeatherSettings)
	{
		if (EditorWeatherType == EditorWeather_WeatherPreset && EditorWeatherPreset)
		{
			SetWeatherSettings(EditorWeatherPreset->GetWeatherPresetSettings());
		}
		else if (EditorWeatherType == EditorWeather_WeatherSettings)
		{
			SetWeatherSettings(EditorWeatherSettings);
		}
	}

	if (bIndependentWindControl)
	{
		SetWindIndependentSettings(EditorIndependentWindSettings);
	}

//	UE_LOG(LogTemp, Warning, TEXT("Updated Settings"));
}

// Almost the same as UpdateSettings(), but with less setup routines
void ASkyCreator::UpdateSettingsSequencer()
{
	if (bUseEditorTimeOfDay)
	{
		SetTime(EditorTimeOfDay);
	}

	if (bUseEditorWeatherSettings)
	{
		if (EditorWeatherType == EditorWeather_WeatherPreset && EditorWeatherPreset)
		{
			SetWeatherSettings(EditorWeatherPreset->GetWeatherPresetSettings());
		}
		else if (EditorWeatherType == EditorWeather_WeatherSettings)
		{
			SetWeatherSettings(EditorWeatherSettings);
		}
	}

	if (bIndependentWindControl)
	{
		SetWindIndependentSettings(EditorIndependentWindSettings);
	}

	if (VolumetricCloud)
	{
		VolumetricCloud->SetLayerBottomAltitude(LayerBottomAltitude);
		VolumetricCloud->SetLayerHeight(LayerHeight);
	}

	if (VolumetricCloudsMPC)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, VolumetricCloudsMPC, "Cloud Map Offset", FLinearColor(CloudMapOffset.X, CloudMapOffset.Y, 0));
	}

	if (SunLight)
	{
		if (bSunConstantIntensity)
		{
			SunLight->SetIntensity(SunIntensity);
		}
		SunLight->LightSourceAngle = SunDiskSize;
		SunLight->AtmosphereSunDiskColorScale = SunAtmosphereDiskColorScale;
	}

	if (MoonLight)
	{
		if (bMoonConstantIntensity)
		{
			MoonLight->SetIntensity(MoonIntensity);
		}
		MoonLight->LightSourceAngle = MoonDiskSize;
		MoonLight->AtmosphereSunDiskColorScale = MoonAtmosphereDiskColorScale;
	}

	if (CommonMPC)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Angular Diameter", MoonDiskSize);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Rotation", MoonRotation);
		UKismetMaterialLibrary::SetScalarParameterValue(this, CommonMPC, "Moon Phase", MoonPhase);
		UKismetMaterialLibrary::SetVectorParameterValue(this, CommonMPC, "Star Map Additional Rotation", FLinearColor(StarMapAdditionalRotation));
	}
}

void ASkyCreator::SetSunSimplePositionSettings(const float InSunriseTime, const float InSunsetTime, const float InSunElevation, const float InSunAzimuth)
{
	if (SunPositionType == SunPositionType_Simple)
	{
		SunriseTime = InSunriseTime;
		SunsetTime = InSunsetTime;
		SunElevation = InSunElevation;
		SunAzimuth = InSunAzimuth;
	}
}

void ASkyCreator::SetMoonSimplePositionSettings(const float InMoonriseTime, const float InMoonsetTime, const float InMoonElevation, const float InMoonAzimuth)
{
	if (MoonPositionType == MoonPositionType_Simple)
	{
		MoonriseTime = InMoonriseTime;
		MoonsetTime = InMoonsetTime;
		MoonElevation = InMoonElevation;
		MoonAzimuth = InMoonAzimuth;
	}
	else if (MoonPositionType == MoonPositionType_SimpleFixedElevation)
	{
		MoonElevation = InMoonElevation;
		MoonAzimuth = InMoonAzimuth;
	}
}

void ASkyCreator::SetRealPositionSettings(const float InLatitude, const float InLongitude, const float InTimeZone, const bool InbDaylightSavingTime, const int InYear, const int InMonth, const int InDay)
{
	if (SunPositionType == SunPositionType_Real || MoonPositionType == MoonPositionType_Real)
	{
		Latitude = InLatitude;
		Longitude = InLongitude;
		TimeZone = InTimeZone;
		bDaylightSavingTime = InbDaylightSavingTime;
		Year = InYear;
		Month = InMonth;
		Day = InDay;
	}
}

void ASkyCreator::PostActorCreated()
{
	Super::PostActorCreated();

}

void ASkyCreator::OnConstruction(const FTransform& Transform)
{
	UpdateSettings();
}

#if WITH_EDITOR

void ASkyCreator::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	/*
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName& PropertyName = PropertyChangedEvent.Property->GetFName();
		
		if (PropertyName == "EditorTimeOfDay")
		{
			SetTime(EditorTimeOfDay);
		}
		
	}
	*/
}

void ASkyCreator::PostEditChange()
{
	Super::PostEditChange();
}
#endif

void ASkyCreator::SetEditorWeatherSettings(FSkyCreatorWeatherSettings NewValue)
{
	UpdateSettings();
}
/*
void ASkyCreator::SetbRealTimeCapture(bool NewValue)
{
	UpdateSettings();
	
	if (SkyLight && !NewValue)
	{
		SkyLight->RecaptureSky();
	}
}
*/
#define SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(MemberType, MemberName) void ASkyCreator::Set##MemberName(MemberType NewValue)\
{\
	if (MemberName != NewValue)\
	{\
		MemberName = NewValue;\
		UpdateSettings();\
	}\
}\

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, EditorTimeOfDay);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(USkyCreatorWeatherPreset*, EditorWeatherPreset);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunriseTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunsetTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunDawnOffsetTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunDuskOffsetTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunElevation);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunAzimuth);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunMinAngleAtDawnDusk);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonriseTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonsetTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonElevation);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonAzimuth);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, Latitude);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, Longitude);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, TimeZone);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(bool, bDaylightSavingTime);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(int32, Year);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(int32, Month);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(int32, Day);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, LayerBottomAltitude);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, LayerHeight);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(FVector2D, CloudMapOffset);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunIntensity);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, SunDiskSize);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(FLinearColor, SunAtmosphereDiskColorScale);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonIntensity);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonDiskSize);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonRotation);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(float, MoonPhase);
SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(FLinearColor, MoonAtmosphereDiskColorScale);

SKYCREATOR_DECLARE_SEQUENCER_SETFUNCTION(FVector, StarMapAdditionalRotation);

void ASkyCreator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyCreator, TimeOfDay);
	DOREPLIFETIME(ASkyCreator, WeatherSettings);
}

void ASkyCreator::OnRep_UpdateTime()
{
	SetTime(TimeOfDay);
}

void ASkyCreator::OnRep_UpdateWeather()
{
	SetWeatherSettings(WeatherSettings);
}