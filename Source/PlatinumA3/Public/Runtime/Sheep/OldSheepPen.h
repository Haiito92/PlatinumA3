// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OldSheepPen.generated.h"

class UBoxComponent;

UCLASS()
class PLATINUMA3_API AOldSheepPen : public AActor
{
	GENERATED_BODY()
	
#pragma region Defaults

	// Sets default values for this actor's properties
	AOldSheepPen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion 

	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> SheepDetectionBox;

	UFUNCTION()
	void OnSheepDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									  const FHitResult& SweepResult);
};
