// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_StateDatas.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EStatsTypes : uint8
{
	Default = 0,
	Float,
	Boolean
};


UCLASS()
class STATEMACHINE_API UPDA_StateDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	TMap<FString, float> FloatVariables;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	TMap<FString, bool> BooleanVariables;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Data")
	float StateSpeed;


public:
	float GetFloatVariable(FString variable) const;
	bool GetBooleanVariable(FString variable) const;

};
