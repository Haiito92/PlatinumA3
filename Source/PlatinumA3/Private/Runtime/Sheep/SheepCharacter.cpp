// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Sheep/SheepCharacter.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/WoolsomeCharactersSettings.h"
#include "Runtime/Sheep/SheepCharacterData.h"
#include "Windows/AllowWindowsPlatformTypes.h"


// Sets default values
ASheepCharacter::ASheepCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionCollision = CreateDefaultSubobject<USphereComponent>("Detection Collision");
	DetectionCollision->SetupAttachment(RootComponent);
	
	CanMove = true;
}

// Called when the game starts or when spawned
void ASheepCharacter::BeginPlay()
{
	Super::BeginPlay();

	const UWoolsomeCharactersSettings* WCSettings = GetDefault<UWoolsomeCharactersSettings>();
	if(WCSettings == nullptr) return;

	const USheepCharacterData* SheepCharacterData = WCSettings->SheepCharacterData.LoadSynchronous();
	if(SheepCharacterData == nullptr) return;

	SetActorClassToFleeFrom(SheepCharacterData->ActorClassToFleeFrom);
	
	DetectionCollision->SetSphereRadius(SheepCharacterData->DetectionRadius);

	SetRallyTime(SheepCharacterData->RallyTime);
	SetSheepRallySpeed(SheepCharacterData->SheepRallySpeed);
	
	SetFleeingDistance(SheepCharacterData->FleeingDistance);
	SetSheepFleeSpeed(SheepCharacterData->SheepFleeSpeed);

	SetSheepWalkSpeed(SheepCharacterData->SheepWalkSpeed);
	GetCharacterMovement()->MaxWalkSpeed = SheepCharacterData->SheepWalkSpeed;
}

// Called every frame
void ASheepCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASheepCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASheepCharacter::KillSheep()
{
	KillEvent.Broadcast();
}

#pragma region Getters/Setters

bool ASheepCharacter::GetCanMove() const
{
	return CanMove;
}

void ASheepCharacter::SetCanMove(bool Value)
{
	CanMove = Value;
	CanMoveChangedEvent.Broadcast(CanMove);
}

float ASheepCharacter::GetRallyTime() const
{
	return RallyTime;
}

void ASheepCharacter::SetRallyTime(float Value)
{
	RallyTime = FMathf::Max(0.f, Value);
}

float ASheepCharacter::GetSheepRallySpeed() const
{
	return SheepRallySpeed;
}

void ASheepCharacter::SetSheepRallySpeed(float Value)
{
	SheepRallySpeed = FMathf::Max(50.f, Value);
}

TSubclassOf<AActor> ASheepCharacter::GetActorClassToFleeFrom() const
{
	return ActorClassToFleeFrom;
}

void ASheepCharacter::SetActorClassToFleeFrom(TSubclassOf<AActor> Subclass)
{
	if(Subclass == nullptr) return;

	ActorClassToFleeFrom = Subclass;
}

float ASheepCharacter::GetFleeingDistance() const
{
	return FleeingDistance;
}

void ASheepCharacter::SetFleeingDistance(float Value)
{
	FleeingDistance = FMathf::Max(0.f, Value);
}

float ASheepCharacter::GetSheepFleeSpeed() const
{
	return SheepFleeSpeed;
}

void ASheepCharacter::SetSheepFleeSpeed(float Value)
{
	SheepFleeSpeed = FMathf::Max(50.f, Value);
}

float ASheepCharacter::GetSheepWalkSpeed() const
{
	return SheepWalkSpeed;
}

void ASheepCharacter::SetSheepWalkSpeed(float Value)
{
	SheepWalkSpeed = FMathf::Max(50.f, Value);
}
#pragma endregion 
