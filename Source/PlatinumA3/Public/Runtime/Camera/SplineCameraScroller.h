﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WoolCineCameraActor.h"
#include "Characters/StateCharacter.h"
#include "GameFramework/Actor.h"
#include "SplineCameraScroller.generated.h"

class USplineComponent;
class AWoolStateCharacter;

UCLASS()
class PLATINUMA3_API ASplineCameraScroller : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASplineCameraScroller();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller")
	TObjectPtr<USplineComponent> m_SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller")
	TObjectPtr<AActor> m_CameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller", Meta = (Interpolate))
	bool m_IsInCinematic = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller")
	float m_CameraMoveSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller")
	float m_CameraRotationSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplineCameraScroller")
	float m_BlendingTime = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, Category = "SplineCameraScroller")
	FVector Direction;
	
	UPROPERTY(BlueprintReadWrite, Category = "SplineCameraScroller")
	TObjectPtr<AWoolStateCharacter> m_Berger;

	UPROPERTY(BlueprintReadWrite, Category = "SplineCameraScroller")
	TObjectPtr<AWoolStateCharacter> m_Chien;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void InitializedSplineCameraScroller(TArray<AStateCharacter*> Characters);

	UFUNCTION()
	void UpdateSplineCamera();

	UFUNCTION()
	void UpdateCharactersInFrustrum(ACharacter* Character);
	
	void LookAtTargetSmooth(FVector TargetLocation);


	UFUNCTION(BlueprintCallable)
	void EnterCinematic();
	
	UFUNCTION(BlueprintCallable)
	void ExitCinematic();
};
