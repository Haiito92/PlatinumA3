// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/CheatsSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

void UCheatsSubsystem::InitSubsystem()
{
	// UE_LOGFMT(LogTemp, Log, "Init Cheats");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheatTeleportPoint::StaticClass(),FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ACheatTeleportPoint* CheatTeleportPoint = Cast<ACheatTeleportPoint>(FoundActor);
		if(CheatTeleportPoint == nullptr) continue;

		CheatTeleportPoints.Add(CheatTeleportPoint);
	}
}

const TArray<TObjectPtr<ACheatTeleportPoint>>& UCheatsSubsystem::GetCheatTeleportPoints() const
{
	return CheatTeleportPoints;
}
