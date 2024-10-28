// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SheepCharacter.generated.h"

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

	UPROPERTY()
	FKillEvent KillEvent;

private:
	bool CanMove;
public:
	UFUNCTION(BlueprintCallable)
	bool GetCanMove() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool Value);
};
