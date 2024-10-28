// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorFunctionLibrary.h"
#include "Containers/UnrealString.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Runtime/ApplicationCore/Public/HAL/PlatformApplicationMisc.h"
#include "Runtime/ApplicationCore/Public/GenericPlatform/GenericWindow.h"

#define RADEG       (180.0 / PI)           // convert RAD to DEG
#define DEGRAD      (PI / 180.0)           // convert DEG to RAD
#define HOURAD      (15.0 * DEGRAD)         // convert to HOUR RADI
#define fromhour(x) (x * 15.0)              // convert HOUR to DEGREES
#define tohour(x)   (x / 15.0)              // convert DEGREES to HOUR
#define sind(x)     sin((x) * DEGRAD)
#define cosd(x)     cos((x) * DEGRAD)
#define tand(x)     tan((x) * DEGRAD)
#define asind(x)    (RADEG * asin(x))
#define acosd(x)    (RADEG * acos(x))
#define atand(x)    (RADEG * atan(x))
#define atan2d(y,x) (RADEG * atan2((y),(x)))

FCelestialPositionData USkyCreatorFunctionLibrary::GetRealSunPosition(const float Latitude, const float Longitude, const float TimeZone, const bool bIsDaylightSavingTime, const FDateTime DateTime)
{
	FCelestialPositionData SunPositionData;

	float TimeOffset = TimeZone;
	if (bIsDaylightSavingTime)
	{
		TimeOffset += 1.0f;
	}

	// Universal Time
	double UT = (DateTime.GetTimeOfDay().GetTotalHours() - TimeOffset); // convert time to float time

	// Julian Day
	double d = (DateTime.GetJulianDay() - 2451543.5) + UT / 24.0;

	/* Orbital elements of the Sun: */
	double w = Rev(282.9404 + 4.70935E-5 * d); // longitude of perihelion
	double a = 1.000000; // mean distance, a.u.
	double e = 0.016709 - 1.151E-9 * d;  // eccentricity
	double M = Rev(356.0470 + 0.9856002585 * d); // mean anomaly

	double oblecl = Rev(23.4393 - 3.563E-7 * d); // obliquity

	double L = Rev(w + M); // mean longitude

	/* The position of the Sun */

	// First, compute the eccentric anomaly E from the mean anomaly M and from the eccentricity e(E and M in degrees) :
	double E = M + e * (180.0 / PI) * sind(M) * (1.0 + e * cosd(M));

	//	Then compute the Sun's distance r and its true anomaly v from:
	double x = cosd(E) - e;
	double y = sqrt(1.0 - e * e) * sind(E);
	double z = 0.0;

	double v = atan2d(y, x); // *RADEG;
	double r = sqrt(x * x + y * y);

	// Now, compute the Sun's true longitude:
	double lonsun = Rev(v + w);

	// Convert lonsun,r to ecliptic rectangular geocentric coordinates xs,ys:
	x = r * cosd(lonsun);
	y = r * sind(lonsun);
	//	z = 0.0;

		// To convert this to equatorial, rectangular, geocentric coordinates, compute:
	double xe = x;
	double ye = y * cosd(oblecl) - 0.0 * sind(oblecl);
	double ze = y * sind(oblecl) + 0.0 * cosd(oblecl);

	// Finally, compute the Sun's Right Ascension (RA) and Declination (Dec):
	double RA = tohour(atan2d(ye, xe));
	double Dec = atan2d(ze, sqrt(xe * xe + ye * ye));
	//	double Dec = asind(ze / r);

		/* The Sidereal Time */

	double GMST0 = Rev(L + 180) / 15;
	double LST = GMST0 + UT + (Longitude / 15);

	/* Azimuthal coordinates */

	// Hour Angle
	float HA = LST - RA;
	float HAd = fromhour(HA);

	// Now it's time to convert our objects HA and Decl to local azimuth and altitude. To do that, we also must know lat, our local latitude. Then we proceed as follows:
	x = cosd(HAd) * cosd(Dec);
	y = sind(HAd) * cosd(Dec);
	z = sind(Dec);

	float xhor = x * sind(Latitude) - z * cosd(Latitude);
	float yhor = y;
	float zhor = x * cosd(Latitude) + z * sind(Latitude);

	SunPositionData.Azimuth = atan2d(yhor, xhor) + 180.0;
	SunPositionData.Elevation = asind(zhor);
/*
	// altitude above the horizon
	float h = 90.833; // 90.833 -0.833

	LST = RA;
	RA = GMST0 + UT * 15.0 + Longitude;

//	double UT = (DateTime.GetTimeOfDay().GetTotalHours() - TimeOffset);
	double UT_Sun_in_south = (RA - GMST0 - Longitude) / 15.0;

	double LHA = (sind(h) - sind(Latitude) * sind(Dec)) / (cosd(Latitude) * cosd(Dec));
	LHA = cosd(LHA);

	double sunset = LHA + UT_Sun_in_south;
	double sunrise = LHA - UT_Sun_in_south;


	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("SUN: UT_Sun_in_south = %f, LHA = %f"), UT_Sun_in_south, LHA));
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("SUN: sunset = %f, sunrise = %f"), sunset, sunrise));
	*/
	return SunPositionData;
}

FCelestialPositionData USkyCreatorFunctionLibrary::GetRealMoonPosition(const float Latitude, const float Longitude, const float TimeZone, const bool bIsDaylightSavingTime, const FDateTime DateTime)
{
	FCelestialPositionData MoonPositionData;

	float TimeOffset = TimeZone;
	if (bIsDaylightSavingTime)
	{
		TimeOffset += 1.0f;
	}

	// Universal Time
	double UT = (DateTime.GetTimeOfDay().GetTotalHours() - TimeOffset); // convert time to float time

	// Julian Day
	double d = (DateTime.GetJulianDay() - 2451543.5) + UT / 24.0;

	/* Orbital elements of the Moon: */
	double N = Rev(125.1228 - 0.0529538083 * d);
	double i = Rev(5.1454);
	double w = Rev(318.0634 + 0.1643573223 * d);
	double a = 60.2666; //  (Earth radii)
	double e = 0.054900;
	double M = Rev(115.3654 + 13.0649929509 * d);

	// Related orbital elements are:
	double w1 = N + w; // longitude of perihelion
	double L = M + w1; // mean longitude

	// eccentric anomaly
//	double E = M + (180 / PI) * sind(M) * (1.0 + e * cosd(M));
	double E = M + (180 / PI) * e * sind(M) * (1.0 + e * cosd(M));
	E = Rev(E);

	// now iterate until difference between E0 and E1 is less than 0.005_deg
	// use E0, calculate E1
	double Ebeforeit = E;
	double Eafterit = 0;
	double E_ErrorBefore = 0;
	int Iterations = 0;
	float E_error = 9;

	while ((E_error > 0.0005) && (Iterations < 20))  // ok - itererer korrekt
	{
		Iterations += 1;
		double E0 = E;
		double E1 = E0 - (E0 - (180 / PI) * e * sind(E0) - M) / (1 - e * cosd(E0));
		E = Rev(E1);
		Eafterit = E;

		if (E < E0) { E_error = E0 - E; }
		else { E_error = E - E0; };


		if (E < Ebeforeit) { E_ErrorBefore = Ebeforeit - E; }
		else { E_ErrorBefore = E - Ebeforeit; };
	}

	// Now compute the planet's distance and true anomaly:
	double x = a * (cosd(E) - e);
	double y = a * sqrt(1.0 - e * e) * sind(E);
	double z = 0.0;

	double r = sqrt(x * x + y * y);
	double v = atan2d(y, x); // *RADEG;

	double xeclip = r * (cosd(N) * cosd(v + w) - sind(N) * sind(v + w) * cosd(i));
	double yeclip = r * (sind(N) * cosd(v + w) + cosd(N) * sind(v + w) * cosd(i));
	double zeclip = r * sind(v + w) * sind(i);

	double moon_longitude = Rev(atan2d(yeclip, xeclip));
	double moon_latitude = atan2d(zeclip, sqrt(xeclip * xeclip + yeclip * yeclip));
	r = sqrt(xeclip * xeclip + yeclip * yeclip + zeclip * zeclip);

	// OK so far

	// Perbutations Moons Longitude

	/* Orbital elements of the Sun: */
	double wss = Rev(282.9404 + 4.70935E-5 * d); // longitude of perihelion
//	double a = 1.000000; // mean distance, a.u.
//	double e = 0.016709 - 1.151E-9 * d;  // eccentricity
	double Mss = Rev(356.0470 + 0.9856002585 * d); // mean anomaly

//	sunangles = sun_angles(d, SiteLon, SiteLat);
	double Ls = Rev(wss + Mss); // Suns mean longitude  er feil
	double Lm = Rev(N + w + M); // moon mean longitude
	double Ms = Mss; // Suns mean anomaly
	double Mm = Rev(M);            // Moons mean anomaly
	double D = Rev(Lm - Ls); //Moons mean elongation
	double F = Rev(Lm - N);  //Moons argument of latitude

	// Perturbations in longitude(degrees)
	double P_lon1 = -1.274 * sind(Mm - 2 * D);  //  (Evection)
	double P_lon2 = +0.658 * sind(2 * D);     //    (Variation)
	double P_lon3 = -0.186 * sind(Ms);    //    (Yearly equation)
	double P_lon4 = -0.059 * sind(2 * Mm - 2 * D);
	double P_lon5 = -0.057 * sind(Mm - 2 * D + Ms);
	double P_lon6 = +0.053 * sind(Mm + 2 * D);
	double P_lon7 = +0.046 * sind(2 * D - Ms);
	double P_lon8 = +0.041 * sind(Mm - Ms);
	double P_lon9 = -0.035 * sind(D);     //      (Parallactic equation)
	double P_lon10 = -0.031 * sind(Mm + Ms);
	double P_lon11 = -0.015 * sind(2 * F - 2 * D);
	double P_lon12 = +0.011 * sind(Mm - 4 * D);

	// Perturbations in latitude(degrees)
	double P_lat1 = -0.173 * sind(F - 2 * D);
	double P_lat2 = -0.055 * sind(Mm - F - 2 * D);
	double P_lat3 = -0.046 * sind(Mm + F - 2 * D);
	double P_lat4 = +0.033 * sind(F + 2 * D);
	double P_lat5 = +0.017 * sind(2 * Mm + F);

	double P_lon = P_lon1 + P_lon2 + P_lon3 + P_lon4 + P_lon5 + P_lon6 + P_lon7 + P_lon8 + P_lon9 + P_lon10 + P_lon11 + P_lon12;
	double P_lat = P_lat1 + P_lat2 + P_lat3 + P_lat4 + P_lat5;
	double P_moondistance = -0.58 * cosd(Mm - 2 * D) - 0.46 * cosd(2 * D);

	moon_longitude = moon_longitude + P_lon;
	moon_latitude = moon_latitude + P_lat;
	r = r + P_moondistance;

	double oblecl = 23.4393 - 3.563E-7 * d; // = 23.4406_deg

	// To simplify we set r = 1.0 
//	r = 1.0;

	// Moon's rectangular ecliptic coordinates
	xeclip = r * cosd(moon_longitude) * cosd(moon_latitude);
	yeclip = r * sind(moon_longitude) * cosd(moon_latitude);
	zeclip = r * sind(moon_latitude);

	// rotate to rectangular equatorial coordinates
	double xequat = xeclip;
	double yequat = yeclip * cosd(oblecl) - zeclip * sind(oblecl);
	double zequat = yeclip * sind(oblecl) + zeclip * cosd(oblecl);

	//	double RA = rev(atan2d(yeclip, xeclip));
	//	double Decl = atan2d(zeclip, sqrt(xeclip * xeclip + yeclip * yeclip));

	double RA = tohour(atan2d(yequat, xequat));
	double Decl = atan2d(zequat, sqrt(xequat * xequat + yequat * yequat));

	// OK so far

	//*********CALCULATE TIME *********************

	double GMST0 = tohour(Rev(Ls + 180));
	double LST = GMST0 + UT + tohour(Longitude);

	// Hour Angle
	float HA = LST - RA;
	float HAd = fromhour(HA);

	// Now it's time to convert our objects HA and Decl to local azimuth and altitude. To do that, we also must know lat, our local latitude. Then we proceed as follows:
	x = cosd(HAd) * cosd(Decl);
	y = sind(HAd) * cosd(Decl);
	z = sind(Decl);

	float xhor = x * sind(Latitude) - z * cosd(Latitude);
	float yhor = y;
	float zhor = x * cosd(Latitude) + z * sind(Latitude);

	// geocentric Moon position, i.e. as seen by an imaginary observer at the center of the Earth
	double Moon_Azimuth = atan2d(yhor, xhor) + 180.0;
	//double Moon_Elevation = asind(zhor);
	double Moon_Elevation = atan2d(zhor, sqrt(xhor * xhor + yhor * yhor));

	MoonPositionData.Azimuth = Moon_Azimuth;
	MoonPositionData.Elevation = Moon_Elevation;
	//	SunPositionData.Elevation = Moon_Elevation - asind(1/r * cos(Moon_Elevation));

		/*
		// The Moon's topocentric position
		double mpar = asind(1 / r);

		double alt_geoc = moon_latitude;
		double alt_topoc = alt_geoc - mpar * cosd(alt_geoc); // alt_geoc = geocentric altitude (for moon?)

		double gclat = Latitude;
		double rho = 1.0;

		gclat = Latitude - 0.1924 * sind(2 * Latitude);
		rho = 0.99833 + 0.00167 * cosd(2 * Latitude);

	//	HA = LST - RA

		double g = atand(tand(gclat) / cosd(HAd));

		double topRA = RA - mpar * rho * cosd(gclat) * sind(HAd) / cosd(Decl);
		double topDecl = Decl - mpar * rho * sind(gclat) * sind(g - Decl) / sind(g);

		// Now it's time to convert our objects HA and Decl to local azimuth and altitude. To do that, we also must know lat, our local latitude. Then we proceed as follows:
		x = cosd(HAd) * cosd(topDecl);
		y = sind(HAd) * cosd(topDecl);
		z = sind(topDecl);

		xhor = x * sind(Latitude) - z * cosd(Latitude);
		yhor = y;
		zhor = x * cosd(Latitude) + z * sind(Latitude);

		// geocentric Moon position, i.e. as seen by an imaginary observer at the center of the Earth
		Moon_Azimuth = atan2d(yhor, xhor) + 180.0;
		Moon_Elevation = asind(zhor) + 180.0;
		//	double Moon_Elevation = atan2d(zhor, sqrt(xhor * xhor + yhor * yhor)) + 180.0;

		SunPositionData.Azimuth = Moon_Azimuth;
		SunPositionData.Elevation = Moon_Elevation;
		//	SunPositionData.Elevation = Moon_Elevation - asind(1/r * cos(Moon_Elevation));
		*/
		//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("MOON: xeclip = %f, yeclip = %f, zeclip = %f"), xeclip, yeclip, zeclip));
		//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("MOON: RA = %f, Decl = %f"), RA, Decl));
	return MoonPositionData;
}

double USkyCreatorFunctionLibrary::ElevationRefraction(double f)
{
	double El_observed;
	double x, a0, a1, a2, a3, a4;

	a0 = 0.58804392;
	a1 = -0.17941557;
	a2 = 0.29906946E-1;
	a3 = -0.25187400E-2;
	a4 = 0.82622101E-4;

	El_observed = f;

	x = FMath::Abs(f + 0.589);
	double refraction = FMath::Abs(a0 + a1 * x + a2 * x * x + a3 * x * x * x + a4 * x * x * x * x);

	if (f > 10.2)
	{
		El_observed = f + 0.01617 * (cosd(FMath::Abs(f)) / sind(FMath::Abs(f)));
	}
	else
	{
		El_observed = f + refraction;
	}
	return El_observed;
}

void USkyCreatorFunctionLibrary::SetConsoleCommandInt(UObject* WorldContextObject, const FString InCommand, uint8 InValue)
{
	const FString ValueString = FString::Printf(TEXT("%d"), InValue);
	const FString Command = InCommand + ValueString;

	UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject, Command);
}

void USkyCreatorFunctionLibrary::SetConsoleCommandFloat(UObject* WorldContextObject, const FString InCommand, float InValue)
{
	const FString ValueString = FString::SanitizeFloat(InValue);
	const FString Command = InCommand + ValueString;

	UKismetSystemLibrary::ExecuteConsoleCommand(WorldContextObject, Command);
}

void USkyCreatorFunctionLibrary::ConvertRenderTargetToTexture2D(UTextureRenderTarget2D* RenderTarget, UTexture2D*& Texture)
{
//	UTexture2D* NewTexture = RenderTarget->ConstructTexture2D(Texture->GetOuter(), Texture->GetName(), RenderTarget->GetMaskedFlags() | RF_Public | RF_Standalone, CTF_Default, NULL);

	if (RenderTarget)
	{
		Texture = RenderTarget->ConstructTexture2D(RenderTarget->GetOuter(), RenderTarget->GetName() + FString("_Tex"), RenderTarget->GetMaskedFlags() | RF_Public | RF_Standalone, CTF_Default, NULL);
	}

	Texture = UTexture2D::CreateTransient(32, 32);
}

UTexture2D* USkyCreatorFunctionLibrary::DynamicConvertRenderTargetToTexture2D(UTextureRenderTarget2D* RenderTarget)
{
//	UTexture2D* Tex2D = RenderTarget->ConstructTexture2D(this, FString("Tex2D"), EObjectFlags::RF_NoFlags);

	UTexture2D* Tex2D = UTexture2D::CreateTransient(32, 32);
	return Tex2D;
}

bool USkyCreatorFunctionLibrary::CheckCloudDensityAtPosition(UObject* WorldContextObject, FVector Position, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget)
{
//	UObject* WorldContext = WorldContext->GetWorld();
//	FVector CurrentPosition;
//	FColor CurrentColor;

	if (ParameterCollection && Material && RenderTarget)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(WorldContextObject, RenderTarget, FLinearColor(0, 0, 0, 1));
//		CurrentPosition = Position;
		UKismetMaterialLibrary::SetVectorParameterValue(WorldContextObject, ParameterCollection, "Cloud Density Sample Position", FLinearColor(Position));
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(WorldContextObject, RenderTarget, Material);
		const FColor CurrentColor = UKismetRenderingLibrary::ReadRenderTargetPixel(WorldContextObject, RenderTarget, 0, 0);
		if (CurrentColor.R != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

float USkyCreatorFunctionLibrary::GetCloudDensityAtPosition(UObject* WorldContextObject, FVector Position, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget)
{
	float Density = 0.0f;

	if (ParameterCollection && Material && RenderTarget)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(WorldContextObject, RenderTarget, FLinearColor(0, 0, 0, 1));
		UKismetMaterialLibrary::SetVectorParameterValue(WorldContextObject, ParameterCollection, "Cloud Density Sample Position", FLinearColor(Position));
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(WorldContextObject, RenderTarget, Material);
		const FColor CurrentColor = UKismetRenderingLibrary::ReadRenderTargetPixel(WorldContextObject, RenderTarget, 0, 0);
		Density = CurrentColor.R;
	}

	return Density;
}

bool USkyCreatorFunctionLibrary::FindLightningPosition(UObject* WorldContextObject, bool bSampleCloudDensity, int32 Samples, UMaterialParameterCollection* ParameterCollection, UMaterialInterface* Material, UTextureRenderTarget2D* RenderTarget, FVector Position, float InnerRadius, float OuterRadius, float MinHeight, float MaxHeight, float DensityThreshold, FVector& OutPosition)
{
	bool Result = false;
	
	FVector Position2D = FVector::ZeroVector;
	FVector ResultPosition = FVector::ZeroVector;

	if (bSampleCloudDensity)
	{
		for (int i = 0; i < Samples; i++) {
			//		FVector CurrentPosition = Position;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::FromInt(i));
			Position2D = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(InnerRadius), USkyCreatorFunctionLibrary::KilometersToCentimeters(OuterRadius)) + Position;
			ResultPosition = FVector(Position2D.X, Position2D.Y, UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(MinHeight), USkyCreatorFunctionLibrary::KilometersToCentimeters(MaxHeight)));
			const float CurrentCloudDensity = GetCloudDensityAtPosition(WorldContextObject, ResultPosition, ParameterCollection, Material, RenderTarget);

			//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, FString::Printf(TEXT("Cloud Sample Position: %s"), *ResultPosition.ToString()));

			if (CurrentCloudDensity >= DensityThreshold) {
				Result = true;
				//UKismetSystemLibrary::DrawDebugPoint(WorldContextObject, ResultPosition, 100.f, FLinearColor::Green, 1.0f);
				OutPosition = ResultPosition;
				break;
			}
			else {
				Result = false;
				//UKismetSystemLibrary::DrawDebugPoint(WorldContextObject, ResultPosition, 50.f, FLinearColor::Red, 1.0f);
			}
		}
	}
	else
	{
		Position2D = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(InnerRadius), USkyCreatorFunctionLibrary::KilometersToCentimeters(OuterRadius)) + Position;
		ResultPosition = FVector(Position2D.X, Position2D.Y, UKismetMathLibrary::RandomFloatInRange(USkyCreatorFunctionLibrary::KilometersToCentimeters(MinHeight), USkyCreatorFunctionLibrary::KilometersToCentimeters(MaxHeight)));
		Result = true;
		OutPosition = ResultPosition;
	}

	return Result;
}

void USkyCreatorFunctionLibrary::CreateAndAssignMID(UObject* WorldContextObject, UMaterialInterface* InMaterial, UMaterialInstanceDynamic*& InMID)
{
	if (InMaterial)
	{
		if (!InMID || (InMID->Parent != InMaterial))
		{
			FString MIDString = InMaterial->GetName();
			MIDString.Append("_MID");
			FName MIDName = FName(*MIDString);
			InMID = UMaterialInstanceDynamic::Create(InMaterial, WorldContextObject, MIDName);
		}
	}
}

float USkyCreatorFunctionLibrary::CentimetersToKilometers(const float Value)
{
	return Value / 100000.0f;
}

float USkyCreatorFunctionLibrary::KilometersToCentimeters(const float Value)
{
	return Value * 100000.0f;
}

bool USkyCreatorFunctionLibrary::IsApplicationForegroundNow()
{
	return FPlatformApplicationMisc::IsThisApplicationForeground();
}