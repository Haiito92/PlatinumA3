// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ScaryComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API UScaryComponent : public USphereComponent
{
	GENERATED_BODY()
#pragma region UnrealDefaults
public:
	// Sets default values for this component's properties
	UScaryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region Scary Radius
protected:
	UFUNCTION()
	void OnDetectionBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY()
	int NbScarableInsideRadius;

	
#pragma endregion

#pragma region Bark
protected:
	UFUNCTION()
	void Bark();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveBark();
	UFUNCTION(BlueprintImplementableEvent)
	void JuicyBark();

private:
	UPROPERTY()
	float BarkMinCooldown;
	UPROPERTY()
	float BarkMaxCooldown;
	UPROPERTY()
	float BarkTimer;
#pragma endregion 
};
