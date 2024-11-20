// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SmoothRotationComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API USmoothRotationComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	USmoothRotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region SmoothRotation
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SmoothRotate(const FRotator& InGoalRotation);
	
protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> RotationCurve;
	
	UPROPERTY()
	FTimeline SmoothRotateTimeline;

	UPROPERTY()
	FRotator StartRotation;
	UPROPERTY()
	FRotator GoalRotation;

	UFUNCTION()
	void SetOwnerRotation();
#pragma endregion 
};
