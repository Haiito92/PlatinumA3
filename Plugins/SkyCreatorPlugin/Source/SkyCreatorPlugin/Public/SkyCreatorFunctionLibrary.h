// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "SkyCreatorFunctionLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SKYCREATORPLUGIN_API FCelestialPositionData
{
    GENERATED_BODY()

public:

	/** Celestial Elevation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
	float Elevation = 0.0f;

	/** Celestial Elevation, corrected for atmospheric diffraction */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
//	float CorrectedElevation = 0.0f;

	/** Celestial azimuth */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
	float Azimuth = 0.0f;

	/** Celestial rise time */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
//	FTimespan RiseTime = 0;

	/** Celestial set time */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
//	FTimespan SetTime = 0;

	/** Celestial noon */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Celestial Position")
//	FTimespan NoonTime = 0;
};

UCLASS()
class SKYCREATORPLUGIN_API USkyCreatorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Sun & Moon Position")
	static FCelestialPositionData GetRealSunPosition(const float Latitude, const float Longitude, const float TimeZone, const bool bIsDaylightSavingTime, const FDateTime DateTime);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Sun & Moon Position")
	static FCelestialPositionData GetRealMoonPosition(const float Latitude, const float Longitude, const float TimeZone, const bool bIsDaylightSavingTime, const FDateTime DateTime);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	static void ConvertRenderTargetToTexture2D(UTextureRenderTarget2D* RenderTarget, UTexture2D* &Texture);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	UTexture2D* DynamicConvertRenderTargetToTexture2D(UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool CheckCloudDensityAtPosition(UObject* WorldContextObject, FVector Position, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget);
	
	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static float GetCloudDensityAtPosition(UObject* WorldContextObject, FVector Position, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool FindLightningPosition(UObject* WorldContextObject, bool bSampleCloudDensity, int32 Samples, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget, FVector Position, float InnerRadius, float OuterRadius, float MinHeight, float MaxHeight, float DensityThreshold, FVector& OutPosition);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void CreateAndAssignMID(UObject* WorldContextObject, UMaterialInterface* InMaterial, UMaterialInstanceDynamic*& InMID);

	/* Convert Centimeters to Kilometers */
	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	static float CentimetersToKilometers(const float Value);

	/* Convert Kilometers to Centimeters */
	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	static float KilometersToCentimeters(const float Value);

	UFUNCTION(BlueprintPure, Category = "Sky Creator|Utility")
	static bool IsApplicationForegroundNow();

	static void SetConsoleCommandInt(UObject* WorldContextObject, const FString InCommand, uint8 InValue);

	static void SetConsoleCommandFloat(UObject* WorldContextObject, const FString InCommand, float InValue);

	/* Desc */
//	UFUNCTION(Category = "Sky Creator|Utility")
//	ASkyCreator* GetSkyCreatorInWorld();

private:

	/* Normalize rotation from 0 to 360 */
	static inline double Rev(const double Value) {
		return Value - floor(Value / 360.0) * 360.0;
	}

	static inline float CalculateJulian(int y, int m, int d, int h = 0) {
		return 367 * y - (7 * (y + ((m + 9) / 12))) / 4 + (275 * m) / 9 + d - 730530;
	}

	/* Returns float decimal hours from 24 hour time */
	static inline double TimeToDecimalHours(int h, int m, int s, int t) {
		return h - t + (m / 60.0) + (s / (60.0 * 60.0));
	}

	UFUNCTION(Category = "Sun Position")
	static double ElevationRefraction(double f);
	
};
