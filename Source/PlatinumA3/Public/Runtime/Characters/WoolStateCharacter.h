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

	virtual void OnLanded(const FHitResult& Hit);
	
#pragma region Feedback Events

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Characters Feedback Events")
	void JUICY_OnWalk();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Characters Feedback Events")
	void JUICY_OnInteract();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Characters Feedback Events")
	void JUICY_OnLanded();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dog Feedback Events")
	void JUICY_OnBark();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dog Feedback Events")
	void JUICY_OnBite();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shepherd Feedback Events")
	void JUICY_OnStartHolding();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shepherd Feedback Events")
	void JUICY_OnStopHolding();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shepherd Feedback Events")
	void JUICY_OnThrowSomething();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shepherd Feedback Events")
	void JUICY_OnRally();
#pragma endregion



	

#pragma region Holding Skill

public:
	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float ThrowStrength = 10000;

	UPROPERTY(EditAnywhere, Category="Launch Fields")
	float LaunchTransTime = 2.0f;
	
	UPROPERTY()
	bool IsHoldingSomething;

	UPROPERTY()
	FName Original_CollisionProfileName;

	UPROPERTY()
	bool Original_SimulatePhysics;


	
	UFUNCTION(BlueprintCallable)
	void StartHolding();

	UFUNCTION(BlueprintCallable)
	void StopHolding(float TransTime);

	UFUNCTION(BlueprintCallable)
	void UpdateHolding();

	UFUNCTION(BlueprintCallable)
	void StartExecuteLaunch(const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void LaunchSomething();
	
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


#pragma region Rally Skill

public:

	UPROPERTY(EditAnywhere, Category="Rally Fields")
	float RallyRadius = 350.0f;
	
	UFUNCTION(BlueprintCallable)
	void LaunchRally();

	UFUNCTION()
	TArray<AActor*> GetSomethingToRally();

#pragma endregion




#pragma region Interacting

public:

	UPROPERTY(EditAnywhere, Category="Interact Fields")
	float InteractRadius = 150.0f;
	
	UFUNCTION(BlueprintCallable)
	void LaunchInteracting();

	UFUNCTION()
	AActor* GetSomethingToInteractWith();

#pragma endregion
};
