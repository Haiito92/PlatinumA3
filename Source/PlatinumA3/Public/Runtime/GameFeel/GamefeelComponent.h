// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamefeelComponent.generated.h"


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API UGamefeelComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region UnrealDefaults
public:
	// Sets default values for this component's properties
	UGamefeelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitGamefeelComponent(USceneComponent* InComponentToModify);

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USceneComponent> ModifiedSceneComponent;
};
