// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

#pragma region Unreal Defaults
// Sets default values
AAIGroupCharacter::AAIGroupCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIGroupCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIGroupCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIGroupCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
#pragma endregion 

void AAIGroupCharacter::InitGroupPawn(int NewIndex)
{
	Index = NewIndex;
}

void AAIGroupCharacter::SetIndex(int NewIndex)
{
	Index = NewIndex;
}

int AAIGroupCharacter::GetIndex() const
{
	return Index;
}

void AAIGroupCharacter::SetIsActivated(bool IsActivated)
{
	bIsActivated = IsActivated;
}

bool AAIGroupCharacter::GetIsActivated() const
{
	return bIsActivated;
}
