// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Sheep/SheepPen.h"

#include "Components/BoxComponent.h"
#include "Runtime/CampaignCore/CampaignGameMode.h"
#include "Runtime/Sheep/SheepCharacter.h"


// Sets default values
ASheepPen::ASheepPen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SheepDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SheepDetectionBox"));
	SheepDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ASheepPen::OnSheepDetectionOverlapBegin);
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

void ASheepPen::OnSheepDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(ASheepCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			FColor::Magenta,
			TEXT("SheepEntered")
			);

		ACampaignGameMode* GameMode = Cast<ACampaignGameMode>(GetWorld()->GetAuthGameMode());

		GameMode->AddSavedSheep(1);
	}
}

