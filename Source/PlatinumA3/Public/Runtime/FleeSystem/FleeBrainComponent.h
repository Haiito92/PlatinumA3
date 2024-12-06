// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FleeFollowerComponent.h"
#include "FleeLeaderComponent.h"
#include "Components/SphereComponent.h"
#include "FleeBrainComponent.generated.h"


class UFleeFollowerComponent;
class UFleeLeaderComponent;

USTRUCT()
struct FFleeBrainInitData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int Index = 0;
	UPROPERTY()
	float LinkRadius = 0.0f;
	UPROPERTY()
	float DetectionRadius = 0.0f;
	UPROPERTY()
	float PostFleeTime = 0.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UFleeBrainComponent : public USphereComponent
{
	GENERATED_BODY()

#pragma region UnrealDefaults
public:
	// Sets default values for this component's properties
	UFleeBrainComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region Initialization
public:
	void InitBrain(const FFleeBrainInitData& InBrainInitData);
	void InitLeaderCortex(const int InIndex, const float InDetectionRadius, const float InPostFleeTime);
	void InitFollowerCortex(const int InIndex);
#pragma endregion 
#pragma region Brain&Links
public:
	UFUNCTION()
	int GetBrainIndex() const;
	UFUNCTION()
	TArray<UFleeBrainComponent*>& GetLinkedBrainComponents();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLinkEvent, UFleeBrainComponent*, Brain, UFleeBrainComponent*, OtherBrain);
	UPROPERTY()
	FLinkEvent LinkEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnlinkEvent, UFleeBrainComponent*, Brain, UFleeBrainComponent*, OtherBrain);
	UPROPERTY()
	FUnlinkEvent UnlinkEvent;
	
protected:
	UFUNCTION()
	void OnLinkDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									 const FHitResult& SweepResult);
	UFUNCTION()
	void OnLinkDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY()
	int BrainIndex;
	
	UPROPERTY()
	TArray<UFleeBrainComponent*> LinkedBrainComponents;
#pragma endregion

#pragma region LeaderCortex
public:
	UFUNCTION()
	UFleeLeaderComponent* GetFleeLeaderComponent() const;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderCortexStartFleeEvent, int, LeaderIndex);
	UPROPERTY()
	FLeaderCortexStartFleeEvent LeaderCortexStartFleeEvent;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderCortexStopFleeEvent, int, LeaderIndex);
	UPROPERTY()
	FLeaderCortexStopFleeEvent LeaderCortexStopFleeEvent;
protected:
	UFUNCTION()
	void OnLeaderCortexStartFleeEvent(int InLeaderIndex);
	UFUNCTION()
	void OnLeaderCortexStopFleeEvent(int InLeaderIndex);
private:
	UPROPERTY()
	TObjectPtr<UFleeLeaderComponent> FleeLeaderComponent;
#pragma endregion

#pragma region FollowerCortex

public:
	UFUNCTION()
	UFleeFollowerComponent* GetFleeFollowerComponent() const;
private:
	UPROPERTY()
	TObjectPtr<UFleeFollowerComponent> FleeFollowerComponent;
#pragma endregion 
};


