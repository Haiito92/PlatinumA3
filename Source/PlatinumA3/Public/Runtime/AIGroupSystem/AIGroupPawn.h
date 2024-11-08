// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIGroupPawn.generated.h"

UCLASS()
class PLATINUMA3_API AAIGroupPawn : public APawn
{
	GENERATED_BODY()
#pragma region Unreal Defaults
public:
	// Sets default values for this pawn's properties
	AAIGroupPawn();

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

public:
	UFUNCTION()
	void SetIndex(int NewIndex);
	UFUNCTION()
	int GetIndex() const;

public:
	void InitGroupPawn(int NewIndex);
};
