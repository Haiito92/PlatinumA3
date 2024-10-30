// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SheepCharacter.generated.h"

class USphereComponent;

UCLASS()
class PLATINUMA3_API ASheepCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Unreal Defaults
public:
	// Sets default values for this character's properties
	ASheepCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void KillSheep();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillEvent);

	UPROPERTY(BlueprintAssignable)
	FKillEvent KillEvent;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCanMoveChangedEvent, bool, CanMove);

	UPROPERTY(BlueprintAssignable)
	FCanMoveChangedEvent CanMoveChangedEvent;
private:
	UPROPERTY()
	bool CanMove;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AActor> ActorClassToFleeFrom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> DetectionCollision;
	
	UPROPERTY(VisibleAnywhere)
	float FleeingDistance = 1000.f;
public:
#pragma region Getters/Setters
	UFUNCTION(BlueprintCallable)
	bool GetCanMove() const;
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Value);

	UFUNCTION(BlueprintCallable)
	//SUCE BIEN FORT ANOINE OH OUIIIII
	TSubclassOf<AActor> GetActorClassToFleeFrom() const;
	UFUNCTION(BlueprintCallable)
	void SetActorClassToFleeFrom(TSubclassOf<AActor> Subclass);
	
	UFUNCTION(BlueprintCallable)
	float GetFleeingDistance() const;
	UFUNCTION(BlueprintCallable)
	void SetFleeingDistance(float Value);
#pragma endregion 
};
