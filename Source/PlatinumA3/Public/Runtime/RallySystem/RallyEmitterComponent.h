// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RallyEmitterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class PLATINUMA3_API URallyEmitterComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	URallyEmitterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region EmitSignal;
public:
	UFUNCTION(BlueprintCallable)
	void EmitSignal();
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void JuicyEmitSignal();
private:
	UPROPERTY()
	float SignalRadius;

	UPROPERTY()
	float RallyEmitCooldown;

	UPROPERTY()
	float RallyTimer;
#pragma endregion 
};
