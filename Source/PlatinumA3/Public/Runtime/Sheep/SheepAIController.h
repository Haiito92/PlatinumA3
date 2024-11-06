// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "SheepAIController.generated.h"

class ASheepCharacter;

UCLASS()
class PLATINUMA3_API ASheepAIController : public AAIController
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this actor's properties
	ASheepAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
#pragma endregion

private:
	TObjectPtr<ASheepCharacter> SheepControlled;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	void BindToSheepEvents();
	
	UFUNCTION()
	void OnCanMoveChangeEvent(bool bCanMove);
	UFUNCTION()
	void OnScareEvent(const AActor* ScaryActor);
	UFUNCTION()
	void OnUnScareEvent();
	UFUNCTION()
	void OnLowScareEvent(FVector LowFleeDirection, float SignalRadius);
};
