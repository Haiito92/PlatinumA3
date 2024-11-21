// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowComponent.generated.h"


class UPhysicsHandleComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UThrowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThrowComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> HoldingTarget;
	
	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float ThrowStrength = 10000;

	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float LaunchTransTime = 2.0f;
	
	UPROPERTY()
	bool IsHoldingSomething;

	UPROPERTY()
	FName Original_CollisionProfileName;

	UPROPERTY()
	bool Original_SimulatePhysics;

public:
	UFUNCTION(BlueprintCallable, Category = "Catchable Interface")
	void Catch(UPhysicsHandleComponent* InPhysicsHandle, USceneComponent* InHoldingTarget);

	UFUNCTION(BlueprintCallable, Category = "Catchable Interface")
	void UpdateHolding();

	UFUNCTION(BlueprintCallable, Category = "Catchable Interface")
	void StopHolding();
	
	UFUNCTION(BlueprintCallable, Category = "Catchable Interface")
	void Launch(bool IsSimulatingPhysic, FName CollisionProfilName, float TransTime);

	UFUNCTION(BlueprintCallable)
	void StartExecuteLaunch(const FHitResult& Hit);
};
