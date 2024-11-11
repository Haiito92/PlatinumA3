// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIGroupPawn.h"

#pragma region Unreal Defaults
// Sets default values
AAIGroupPawn::AAIGroupPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIGroupPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIGroupPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIGroupPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
#pragma endregion 

void AAIGroupPawn::InitGroupPawn(int NewIndex)
{
	Index = NewIndex;
	bIsActivated = true;
}

void AAIGroupPawn::SetIndex(int NewIndex)
{
	Index = NewIndex;
}

int AAIGroupPawn::GetIndex() const
{
	return Index;
}

void AAIGroupPawn::SetIsActivated(bool IsActivated)
{
	bIsActivated = IsActivated;
}

bool AAIGroupPawn::GetIsActivated() const
{
	return bIsActivated;
}