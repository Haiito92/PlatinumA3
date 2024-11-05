// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Berger/Rallyable.h"
#include "Runtime/Scarable/IScarable.h"
#include "SheepCharacter.generated.h"

class UScarableComponent;
enum class ESheepStateID : uint8;
class USphereComponent;

UCLASS()
class PLATINUMA3_API ASheepCharacter : public ACharacter, public IRallyable, public IScarable
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

#pragma region Sheep Defaults
	const float MIN_WALK_SPEED = 0.f;

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

	// UPROPERTY(VisibleAnywhere)
	// ESheepStateID SheepStateID;
	
	UPROPERTY(VisibleAnywhere, Category="SheepDefault")
	float SheepWalkSpeed;
	
public:
	UFUNCTION(BlueprintCallable)
	bool GetCanMove() const;
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Value);

	UFUNCTION(BlueprintCallable)
	void UpdateWalkSpeed(float Value) const;
	
	// UFUNCTION(BlueprintCallable)
	// ESheepStateID GetSheepStateID() const;
	// UFUNCTION(BlueprintCallable)
	// void SetSheepStateID(ESheepStateID ID);

	UFUNCTION(BlueprintCallable)
	float GetSheepWalkSpeed() const;
	UFUNCTION(BlueprintCallable)
	void SetSheepWalkSpeed(float Value);

#pragma endregion 

	

#pragma region Rally
private:
	UPROPERTY(VisibleAnywhere, Category="Rally")
	float RallyTime;

	UPROPERTY(VisibleAnywhere, Category="Rally")
	float SheepRallySpeed;
	
public:
	UFUNCTION(BlueprintCallable)
	float GetRallyTime() const;
	UFUNCTION(BlueprintCallable)
	void SetRallyTime(float Value);
	
	UFUNCTION(BlueprintCallable)
	float GetSheepRallySpeed() const;
	UFUNCTION(BlueprintCallable)
	void SetSheepRallySpeed(float Value);
#pragma endregion

#pragma region Flee
private:
	UPROPERTY(VisibleAnywhere, Category="Flee")
	TSubclassOf<AActor> ActorClassToFleeFrom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> DetectionCollision;
	
	
	UPROPERTY(VisibleAnywhere, Category="Flee")
	float FleeingDistance = 1000.f;

	UPROPERTY(VisibleAnywhere, Category="Flee")
	float SheepFleeSpeed;
public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetActorClassToFleeFrom() const;
	UFUNCTION(BlueprintCallable)
	void SetActorClassToFleeFrom(TSubclassOf<AActor> Subclass);
	
	UFUNCTION(BlueprintCallable)
	float GetFleeingDistance() const;
	UFUNCTION(BlueprintCallable)
	void SetFleeingDistance(float Value);

	UFUNCTION(BlueprintCallable)
	float GetSheepFleeSpeed() const;
	UFUNCTION(BlueprintCallable)
	void SetSheepFleeSpeed(float Value);
private:
	UFUNCTION()
	void OnDetectionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										  const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

#pragma region IScarable
private:
	UPROPERTY()
	TObjectPtr<UScarableComponent> ScarableComponent;
public:
	UScarableComponent* GetScarableComponent() const;
private:
	virtual void Scare_Implementation(const AActor* ScaryActor) override;
	virtual void UnScare_Implementation() override;
	virtual void LowScare_Implementation(FVector LowFleeDirection, float SignalRadius) override;
#pragma endregion 
};

