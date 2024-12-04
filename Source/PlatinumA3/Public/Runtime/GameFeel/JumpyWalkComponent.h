// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpyWalkComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API UJumpyWalkComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region UnrealDefaults
public:
	// Sets default values for this component's properties
	UJumpyWalkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region JumpyWalk
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartJumpyWalk();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopJumpyWalk();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyFootStep();
#pragma endregion 
};
