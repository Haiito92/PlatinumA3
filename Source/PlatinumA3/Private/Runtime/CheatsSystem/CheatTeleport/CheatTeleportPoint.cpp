// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CheatsSystem/CheatTeleport/CheatTeleportPoint.h"

#pragma region UnrealDefaults
// Sets default values
ACheatTeleportPoint::ACheatTeleportPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
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

#pragma endregion 
