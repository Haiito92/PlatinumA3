// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGroupCharacter.generated.h"

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion


private:
	UPROPERTY()
	int Index;

	UPROPERTY()
	bool bIsActivated = true;

public:
	UFUNCTION()
	void SetIndex(int NewIndex);
	UFUNCTION(BlueprintCallable)
	int GetIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetIsActivated(bool IsActivated);
	UFUNCTION(BlueprintCallable)
	bool GetIsActivated() const;
	
public:
	void InitGroupPawn(int NewIndex);
};
