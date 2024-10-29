// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/StateCharacter.h"
#include "WoolStateCharacter.generated.h"


class UPhysicsHandleComponent;
class USceneComponent;


UCLASS()
class PLATINUMA3_API AWoolStateCharacter : public AStateCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWoolStateCharacter();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* HoldingTarget;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Holding Skill

public:
	UPROPERTY()
	bool IsHoldingSomething;

	UPROPERTY()
	FName Original_CollisionProfileName;

	UPROPERTY()
	bool Original_SimulatePhysics;


	
	UFUNCTION(BlueprintCallable)
	void StartHolding();

	UFUNCTION(BlueprintCallable)
	void StopHolding();

	UFUNCTION(BlueprintCallable)
	void UpdateHolding();
	
	UFUNCTION()
	AActor* GetSomethingToHold();


#pragma endregion


#pragma region Bite Skill

public:

	UFUNCTION(BlueprintCallable)
	void LaunchBite();

	UFUNCTION()
	AActor* GetSomethingToBite();

#pragma endregion
};
