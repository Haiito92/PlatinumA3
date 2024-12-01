// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIPawnStateID.h"
#include "GameFramework/Character.h"
#include "AIGroupCharacter.generated.h"

enum class EAIPawnStateID : uint8;

UCLASS()
class PLATINUMA3_API AAIGroupCharacter : public ACharacter
{
	GENERATED_BODY()
#pragma region Unreal Defaults
public:
	// Sets default values for this character's properties
	AAIGroupCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// virtual void Landed(const FHitResult& Hit) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLanded);
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FOnLanded OnLanded;
#pragma endregion


public:
	void InitGroupPawn(int NewIndex);

	UFUNCTION(BlueprintCallable)
	void ActivatePawn();
	UFUNCTION(BlueprintCallable)
	void DisablePawn();
	UFUNCTION(BlueprintCallable)
	void UnActivatePawn();
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveActivatePawn();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveDisablePawn();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReceiveUnActivatePawn();
private:
	UPROPERTY()
	int Index;

	UPROPERTY()
	EAIPawnStateID PawnStateID;

	// UPROPERTY()
	// bool bIsActivated = true;

public:
	UFUNCTION()
	void SetIndex(int NewIndex);
	UFUNCTION(BlueprintCallable)
	int GetIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetPawnStateID(const EAIPawnStateID NewPawnStateID);
	UFUNCTION(BlueprintCallable)
	EAIPawnStateID GetPawnStateID() const;
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveNewStateID(const EAIPawnStateID NewPawnStateID);


public:
	
	UFUNCTION()
	void StartRotateAICharacter(const FRotator& GoalRotation);
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveStartRotateAICharacter(const FRotator& GoalRotation);
	UFUNCTION()
	void StopRotateAICharacter();
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveStopRotateAICharacter();
protected:
	UPROPERTY(BlueprintReadWrite)
	bool RotationHandled = false;
	// UFUNCTION()
	// void RotateCharacter(const FRotator& StartingRotation, const FRotator& GoalRotation, const float Alpha, const bool ShortestPath);
	// UFUNCTION(BlueprintCallable)
	// void SetIsActivated(bool IsActivated);
	// UFUNCTION(BlueprintCallable)
	// bool GetIsActivated() const;
};
