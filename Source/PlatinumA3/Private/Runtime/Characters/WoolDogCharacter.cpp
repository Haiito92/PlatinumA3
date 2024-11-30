// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Characters/WoolDogCharacter.h"


// Sets default values
AWoolDogCharacter::AWoolDogCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWoolDogCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWoolDogCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWoolDogCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

