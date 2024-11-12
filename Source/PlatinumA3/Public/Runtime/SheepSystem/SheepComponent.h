// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SheepComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API USheepComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	USheepComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

#pragma region Sheep
public:
	// UFUNCTION(BlueprintCallable)
	// void CaptureSheep();
public:
	UFUNCTION(BlueprintCallable)
	bool GetIsCaptured() const;
	
	UFUNCTION(BlueprintCallable)
	void SetIsCaptured(bool IsCaptured);
private:
	UPROPERTY()
	bool bIsCaptured;
	
#pragma endregion 
};
