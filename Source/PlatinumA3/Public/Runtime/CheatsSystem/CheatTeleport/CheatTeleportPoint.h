// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheatTeleportPoint.generated.h"

UCLASS()
class PLATINUMA3_API ACheatTeleportPoint : public AActor
{
	GENERATED_BODY()

#pragma region UnrealDefaults
public:
	// Sets default values for this actor's properties
	ACheatTeleportPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	const TObjectPtr<USceneComponent> GetDogTeleportStart() const;
	const TObjectPtr<USceneComponent> GetShepherdTeleportStart() const;
	
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DogTeleportStart;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ShepherdTeleportStart;
};
