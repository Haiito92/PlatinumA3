// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/AIGroupSystem/AIGroupCharacter.h"

#include "Runtime/AIGroupSystem/AIPawnStateID.h"

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

// void AAIGroupCharacter::Landed(const FHitResult& Hit)
// {
// 	Super::Landed(Hit);
//
// 	OnLanded.Broadcast();
// }

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
	PawnStateID = EAIPawnStateID::Activated;
	// bIsActivated = true;
}

void AAIGroupCharacter::ActivatePawn()
{
	SetPawnStateID(EAIPawnStateID::Activated);
}

void AAIGroupCharacter::DisablePawn()
{
	SetPawnStateID(EAIPawnStateID::Disabled);
}

void AAIGroupCharacter::UnActivatePawn()
{
	SetPawnStateID(EAIPawnStateID::UnActivated);

	GEngine->AddOnScreenDebugMessage(
		-1, 3.0f,FColor::Black, TEXT("Unactivate pawn"));
}

void AAIGroupCharacter::SetIndex(int NewIndex)
{
	Index = NewIndex;
}

int AAIGroupCharacter::GetIndex() const
{
	return Index;
}

void AAIGroupCharacter::SetPawnStateID(const EAIPawnStateID NewPawnStateID)
{
	if(PawnStateID == NewPawnStateID) return;
	PawnStateID = NewPawnStateID;
	
}

EAIPawnStateID AAIGroupCharacter::GetPawnStateID() const
{
	return PawnStateID;
}
//
// void AAIGroupCharacter::SetIsActivated(bool IsActivated)
// {
// 	bIsActivated = IsActivated;
// }
//
// bool AAIGroupCharacter::GetIsActivated() const
// {
// 	return bIsActivated;
// }
