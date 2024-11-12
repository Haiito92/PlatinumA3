// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/SheepSystem/SheepPen.h"


// Sets default values
ASheepPen::ASheepPen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASheepPen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASheepPen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

