// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "FleeFollowerComponent.generated.h"


class UFleeLeaderComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLATINUMA3_API UFleeFollowerComponent : public USphereComponent
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults

public:
	// Sets default values for this component's properties
	UFleeFollowerComponent();

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region FleeFollower
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEncounteredNewGroupEvent, int, InGroupLeaderIndex, int, InFollowerIndex);
	UPROPERTY()
	FEncounteredNewGroupEvent EncounteredNewGroupEvent;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLostContactWithGroupEvent, int, GroupLeaderIndex, int, InFollowerIndex);
	UPROPERTY()
	FLostContactWithGroupEvent LostContactWithGroupEvent;
	
	
	UFUNCTION()
	TMap<int,int>& GetGroupFollowedIndexes();

	UFUNCTION()
	int GetFollowerIndex()const;
protected:
	UFUNCTION()
	void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);
	
	UFUNCTION()
	void OnLeaderStartFleeEvent(int LeaderIndex);
	UFUNCTION()
	void OnLeaderStopFleeEvent(int LeaderIndex);
	
	UFUNCTION()
	void IncrementGroupAmount(int InGroupLeaderIndex, int InAmount = 1);
	UFUNCTION()
	void DecrementGroupAmount(int InGroupLeaderIndex, int InAmount = 1);
private:	
	UPROPERTY()
	TArray<UFleeLeaderComponent*> NeighbouringLeaders;
	UPROPERTY()
	TArray<UFleeFollowerComponent*> NeighbouringFollowers;

	UPROPERTY()
	TMap<int, int> GroupFollowedIndexes;

	UPROPERTY()
	int FollowerIndex;
#pragma endregion 
};

