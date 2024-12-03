// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SheepComponent.generated.h"


enum class ESheepStateFlag;
class USheepSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
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
	UFUNCTION(BlueprintCallable)
	void InitSheep(int Index);
	
public:
	UFUNCTION(BlueprintCallable)
	void Birth();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JuicyBirth();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSheepBirthEvent, int, Index);
	UPROPERTY()
	FSheepBirthEvent SheepBirthEvent;
	
	UFUNCTION(BlueprintCallable)
	void Capture();
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void JuicyCapture();
	
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void JuicyDie();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSheepDeathEvent, int, Index);
	UPROPERTY()
	FSheepDeathEvent SheepDeathEvent;
	
public:
	UFUNCTION(BlueprintCallable)
	int GetSheepIndex() const;
	UFUNCTION(BlueprintCallable)
	void SetIndex(const int Index);

	UFUNCTION(BlueprintCallable)
	bool GetIsAlive() const;	
	UFUNCTION(BlueprintCallable)
	void SetIsAlive(const bool InIsAlive);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsCaptured() const;
	UFUNCTION(BlueprintCallable)
	void SetIsCaptured(const bool IsCaptured);

	UFUNCTION(BlueprintCallable)
	const FVector& GetCapturedRoamingLocation() const;
	UFUNCTION(BlueprintCallable)
	void SetCapturedRoamingLocation(const FVector& NewValue);
private:

	UPROPERTY()
	unsigned int SheepIndex;

	UPROPERTY()
	bool bIsAlive;
	
	UPROPERTY()
	bool bIsCaptured;

	UPROPERTY()
	FVector CapturedRoamingLocation;
	
#pragma endregion 
};
