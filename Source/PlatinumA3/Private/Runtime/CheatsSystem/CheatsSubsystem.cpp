// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/CheatsSubsystem.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"
#include "Runtime/Characters/WoolDogCharacter.h"
#include "Runtime/Characters/WoolShepherdCharacter.h"
#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

void UCheatsSubsystem::InitSubsystem()
{
	// UE_LOGFMT(LogTemp, Log, "Init Cheats");

	DogCharacter = Cast<AWoolDogCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(),
		AWoolDogCharacter::StaticClass()));
	ShepherdCharacter = Cast<AWoolShepherdCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(),
		AWoolShepherdCharacter::StaticClass()));
	
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

void UCheatsSubsystem::CheatTeleport(const int Index) const
{
	UE_LOGFMT(LogTemp, Warning, "Cheat Teleport : {0}", Index);
	const ACheatTeleportPoint* CheatTeleportPoint = CheatTeleportPoints[Index];
	if(CheatTeleportPoint==nullptr)return;

	const USceneComponent* DogTeleportStart = CheatTeleportPoint->GetDogTeleportStart();
	if(DogCharacter != nullptr && DogTeleportStart != nullptr)
		DogCharacter->SetActorLocation(DogTeleportStart->GetComponentLocation());

	const USceneComponent* ShepherdTeleportStart = CheatTeleportPoint->GetShepherdTeleportStart();
	if(ShepherdCharacter != nullptr && ShepherdTeleportStart != nullptr)
		ShepherdCharacter->SetActorLocation(ShepherdTeleportStart->GetComponentLocation());
	
}
