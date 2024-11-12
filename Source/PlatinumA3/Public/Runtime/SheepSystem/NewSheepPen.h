// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewSheepPen.generated.h"

class USheepComponent;
class USplineComponent;
class USplineMeshComponent;
class UBoxComponent;

UCLASS()
class PLATINUMA3_API ANewSheepPen : public AActor
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this actor's properties
	ANewSheepPen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region Pen
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pen: Fence")
	TObjectPtr<USplineComponent> PenFenceSpline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pen: Fence")
	TSoftObjectPtr<UStaticMesh> PenFenceMesh;
#pragma endregion 
	
#pragma region SheepPenEntry
private:
	UPROPERTY(EditAnywhere, Category="Pen: Entry")
	TObjectPtr<UBoxComponent> PenEntryCollision;

	UFUNCTION()
	void OnEntryCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									  const FHitResult& SweepResult);
	UFUNCTION()
	void OnEntryCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CaptureSheep(USheepComponent* SheepComponent) const;
	
#pragma endregion 
};
