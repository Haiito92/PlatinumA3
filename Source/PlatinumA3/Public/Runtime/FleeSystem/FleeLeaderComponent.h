// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "FleeLeaderComponent.generated.h"

UENUM()
enum class EFleeLeadState : uint8
{
	None,
	NotFleeing,
	Fleeing,
	PostFleeing
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UFleeLeaderComponent : public USphereComponent
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	UFleeLeaderComponent();

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region FleeLeader
public:
	UFUNCTION()
	void Init(const unsigned InIndex, const float InDetectionRadius, const float InPostFleeTime);
	
	UFUNCTION()
	const TArray<AActor*>& GetScaryActors() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartFleeEvent, int, InLeaderIndex);
	UPROPERTY()
	FStartFleeEvent StartFleeEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostFleeEvent, int, InLeaderIndex);
	UPROPERTY()
	FPostFleeEvent PostFleeEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopFleeEvent, int, InLeaderIndex);
	UPROPERTY()
	FStopFleeEvent StopFleeEvent;

	UFUNCTION()
	bool GetFleeing() const;

	UFUNCTION()
	EFleeLeadState GetFleeLeadState() const;

	UFUNCTION()
	const FVector& GetFleeDirection() const;
	
	UFUNCTION()
	int GetLeaderIndex() const;
protected:
	UFUNCTION()
	void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	UFUNCTION()
	void StartFlee();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyStartFlee();

	UFUNCTION()
	void PostFlee();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyPostFlee();
	
	UFUNCTION()
	void StopFlee();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyStopFlee();

private:
	UPROPERTY()
	FName TagToFleeFrom;
	
	UPROPERTY()
	TArray<AActor*> ScaryActors;

	UPROPERTY()
	bool bFleeing = false;

	UPROPERTY()
	EFleeLeadState FleeLeadState = EFleeLeadState::NotFleeing;

	UPROPERTY()
	FVector FleeDirection;
	
	UPROPERTY()
	float PostFleeTime = 0.0f;
	UPROPERTY()
	float PostFleeTimer = 0.0f;
	
	UPROPERTY()
	unsigned int LeaderIndex;
#pragma endregion 
};
