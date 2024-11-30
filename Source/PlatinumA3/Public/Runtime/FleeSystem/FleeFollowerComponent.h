// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "FleeFollowerComponent.generated.h"


class UFleeLeaderComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UFleeFollowerComponent : public USphereComponent
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults

public:
	// Sets default values for this component's properties
	UFleeFollowerComponent();

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region FleeFollower
private:
	UPROPERTY()
	TArray<UFleeLeaderComponent*> NeighbouringLeaders;
	UPROPERTY()
	TArray<UFleeFollowerComponent*> NeighbouringFollowers;

	UFUNCTION()
	void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

#pragma endregion 
};

