// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Sheep/SheepAIController.h"

#include "Runtime/Scarable/ScarableComponent.h"
#include "Runtime/Sheep/SheepCharacter.h"

#pragma region Unreal Defaults
// Sets default values
ASheepAIController::ASheepAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASheepAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASheepAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASheepAIController::Destroyed()
{
	Super::Destroyed();
	if(SheepControlled != nullptr)
	{
		SheepControlled->CanMoveChangedEvent.RemoveDynamic(this, &ASheepAIController::OnCanMoveChangeEvent);

		UScarableComponent* ScarableComponent = SheepControlled->GetScarableComponent();
		if(ScarableComponent != nullptr)
		{
			ScarableComponent->ScareEvent.RemoveDynamic(this, &ASheepAIController::OnScareEvent);
			ScarableComponent->UnScareEvent.RemoveDynamic(this, &ASheepAIController::OnUnScareEvent);
			ScarableComponent->LowScareEvent.RemoveDynamic(this, &ASheepAIController::OnLowScareEvent);
		}
	}
}

#pragma endregion

void ASheepAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SheepControlled = Cast<ASheepCharacter>(InPawn);

	BindToSheepEvents();

	
}

void ASheepAIController::BindToSheepEvents()
{
	if(SheepControlled == nullptr) return;
	
	SheepControlled->CanMoveChangedEvent.AddDynamic(this, &ASheepAIController::OnCanMoveChangeEvent);

	UScarableComponent* ScarableComponent = SheepControlled->GetScarableComponent();
	if(ScarableComponent == nullptr) return;

	ScarableComponent->ScareEvent.AddDynamic(this, &ASheepAIController::OnScareEvent);
	ScarableComponent->UnScareEvent.AddDynamic(this, &ASheepAIController::OnUnScareEvent);
	ScarableComponent->LowScareEvent.AddDynamic(this, &ASheepAIController::OnLowScareEvent);
}

void ASheepAIController::OnCanMoveChangeEvent(bool bCanMove)
{
	
}

void ASheepAIController::OnScareEvent(const AActor* ScaryActor)
{
	
}

void ASheepAIController::OnUnScareEvent()
{
	
}

void ASheepAIController::OnLowScareEvent(FVector LowFleeDirection, float SignalRadius)
{
	
}
