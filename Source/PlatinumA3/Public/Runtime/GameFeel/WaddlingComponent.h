// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaddlingComponent.generated.h"


UCLASS(Abstract,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API UWaddlingComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region UnrealDefaults
public:
	// Sets default values for this component's properties
	UWaddlingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region Waddling
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartWaddling();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopWaddling();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyWaddling();
#pragma endregion 
};
