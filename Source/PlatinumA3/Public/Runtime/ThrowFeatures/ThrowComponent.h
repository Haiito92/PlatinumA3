﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowComponent.generated.h"


class UPhysicsHandleComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleDynamicDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UThrowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThrowComponent();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnCustomLandedFunc();

	UFUNCTION()
	void OnPhysicsHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void HandleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> HoldingTarget;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintAssignable, Category="Throw Component Delegates")
	FSimpleDynamicDelegate LandingStep_Delegate;

	UPROPERTY(BlueprintAssignable, Category="Throw Component Delegates")
	FSimpleDynamicDelegate Catch_Delegate;

	UPROPERTY(BlueprintAssignable, Category="Throw Component Delegates")
	FSimpleDynamicDelegate Throw_Delegate;

	UPROPERTY(BlueprintAssignable, Category="Throw Component Delegates")
	FSimpleDynamicDelegate Landing_Delegate;
	
	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float ThrowStrength = 2000;

	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float LaunchTransTime = 2.0f;
	
	UPROPERTY()
	bool IsHoldingSomething;

	UPROPERTY()
	FName Original_CollisionProfileName;

	UPROPERTY()
	bool Original_SimulatePhysics;

public:
	UFUNCTION(BlueprintCallable, Category = "Throw Component")
	void Catch(UPhysicsHandleComponent* InPhysicsHandle, USceneComponent* InHoldingTarget);

	UFUNCTION(BlueprintCallable, Category = "Throw Component")
	void UpdateHolding();

	UFUNCTION(BlueprintCallable, Category = "Throw Component")
	void StopHolding();

	UFUNCTION(BlueprintCallable, Category = "Throw Component")
	void CheckForLanding();
	
	UFUNCTION(BlueprintCallable, Category = "Throw Component")
	void Launch();

	UFUNCTION(BlueprintCallable)
	void StartExecuteLaunch(const FHitResult& Hit);
};
