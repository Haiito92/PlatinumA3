// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IScarable.h"
#include "Components/ActorComponent.h"
#include "ScarableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UScarableComponent : public UActorComponent, public IScarable
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	UScarableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region IScarable
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScareEvent, const AActor*, ScaryActor);
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FScareEvent ScareEvent;
	virtual void Scare_Implementation(const AActor* ScaryActor) override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnScareEvent);
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FUnScareEvent UnScareEvent;
	virtual void UnScare_Implementation() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLowScareEvent, FVector, LowFleeDirection, float, SignalRadius);
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FLowScareEvent LowScareEvent;
	virtual void LowScare_Implementation(FVector LowFleeDirection, float SignalRadius) override;
#pragma endregion
};
