// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Sheep/SheepCharacter.h"

#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/WoolsomeCharactersSettings.h"
#include "Runtime/Scarable/IScary.h"
#include "Runtime/Scarable/ScarableComponent.h"
#include "Runtime/Sheep/SheepCharacterData.h"
#include "Runtime/Sheep/SheepStateID.h"
#include "Windows/AllowWindowsPlatformTypes.h"


#pragma region Unreal Defaults
// Sets default values
ASheepCharacter::ASheepCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionCollision = CreateDefaultSubobject<USphereComponent>("Detection Collision");
	DetectionCollision->SetupAttachment(RootComponent);
	
	CanMove = true;


	ScarableComponent = CreateDefaultSubobject<UScarableComponent>("Scarable Component");
}

// Called when the game starts or when spawned
void ASheepCharacter::BeginPlay()
{
	Super::BeginPlay();

	const UWoolsomeCharactersSettings* WCSettings = GetDefault<UWoolsomeCharactersSettings>();
	if(WCSettings == nullptr) return;

	const USheepCharacterData* SheepCharacterData = WCSettings->SheepCharacterData.LoadSynchronous();
	if(SheepCharacterData == nullptr) return;

	DetectionCollision->SetSphereRadius(SheepCharacterData->DetectionRadius);

	SetSheepWalkSpeed(SheepCharacterData->SheepWalkSpeed);
	GetCharacterMovement()->MaxWalkSpeed = SheepCharacterData->SheepWalkSpeed;

	SetRallyTime(SheepCharacterData->RallyTime);
	SetSheepRallySpeed(SheepCharacterData->SheepRallySpeed);
	
	SetActorClassToFleeFrom(SheepCharacterData->ActorClassToFleeFrom);
	SetFleeingDistance(SheepCharacterData->FleeingDistance);
	SetSheepFleeSpeed(SheepCharacterData->SheepFleeSpeed);

	//Bind to Events
	DetectionCollision->OnComponentBeginOverlap.AddDynamic(this, &ASheepCharacter::OnDetectionCollisionBeginOverlap);
	DetectionCollision->OnComponentEndOverlap.AddDynamic(this, &ASheepCharacter::OnDetectionCollisionEndOverlap);
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
#pragma endregion 

#pragma region Sheep Defaults
void ASheepCharacter::KillSheep()
{
	KillEvent.Broadcast();
}
bool ASheepCharacter::GetCanMove() const
{
	return CanMove;
}

void ASheepCharacter::SetCanMove(bool Value)
{
	CanMove = Value;
	CanMoveChangedEvent.Broadcast(CanMove);
}

void ASheepCharacter::UpdateWalkSpeed(float Value) const
{
	GetCharacterMovement()->MaxWalkSpeed = FMathf::Max(MIN_WALK_SPEED, Value);
}

// ESheepStateID ASheepCharacter::GetSheepStateID() const
// {
// 	return SheepStateID;
// }
//
// void ASheepCharacter::SetSheepStateID(ESheepStateID ID)
// {
// 	if(ID == ESheepStateID::None) return;
// 	SheepStateID = ID;
// }

float ASheepCharacter::GetSheepWalkSpeed() const
{
	return SheepWalkSpeed;
}

void ASheepCharacter::SetSheepWalkSpeed(float Value)
{
	SheepWalkSpeed = FMathf::Max(50.f, Value);
}
#pragma endregion 

#pragma region Rally
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
	SheepRallySpeed = FMathf::Max(MIN_WALK_SPEED, Value);
}
#pragma endregion

#pragma region Flee
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
	SheepFleeSpeed = FMathf::Max(MIN_WALK_SPEED, Value);
}

void ASheepCharacter::OnDetectionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UScary>())
	{
		Scare_Implementation(OtherActor);
	}
}

void ASheepCharacter::OnDetectionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->Implements<UScary>())
	{
		UnScare_Implementation();
	}
}

#pragma endregion

#pragma region IScarable
void ASheepCharacter::Scare_Implementation(const AActor* ScaryActor)
{
	IScarable::Scare_Implementation(ScaryActor);

	ScarableComponent->Scare_Implementation(ScaryActor);
}

void ASheepCharacter::UnScare_Implementation()
{
	IScarable::UnScare_Implementation();

	ScarableComponent->UnScare_Implementation();
}

void ASheepCharacter::LowScare_Implementation(FVector LowFleeDirection, float SignalRadius)
{
	IScarable::LowScare_Implementation(LowFleeDirection, SignalRadius);

	ScarableComponent->LowScare_Implementation(LowFleeDirection, SignalRadius);
}
#pragma endregion 