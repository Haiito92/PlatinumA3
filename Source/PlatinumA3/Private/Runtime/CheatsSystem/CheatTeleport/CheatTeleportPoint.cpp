// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

#include "GameFramework/PlayerStart.h"

#pragma region UnrealDefaults
// Sets default values
ACheatTeleportPoint::ACheatTeleportPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent= CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = RootSceneComponent;
	
	DogTeleportStart = CreateDefaultSubobject<USceneComponent>("DogTeleportStart");
	DogTeleportStart->SetupAttachment(RootComponent);
	ShepherdTeleportStart = CreateDefaultSubobject<USceneComponent>("ShepherdTeleportStart");
	ShepherdTeleportStart->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACheatTeleportPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheatTeleportPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const TObjectPtr<USceneComponent> ACheatTeleportPoint::GetDogTeleportStart() const
{
	return DogTeleportStart;
}

const TObjectPtr<USceneComponent> ACheatTeleportPoint::GetShepherdTeleportStart() const
{
	return ShepherdTeleportStart;
}
#pragma endregion 

// const TObjectPtr<APlayerStart> ACheatTeleportPoint::GetDogTeleportStart() const
// {
// 	return DogTeleportStart;
// }
//
// const TObjectPtr<APlayerStart> ACheatTeleportPoint::GetShepherdTeleportStart() const
// {
// 	return ShepherdTeleportStart;
// }

