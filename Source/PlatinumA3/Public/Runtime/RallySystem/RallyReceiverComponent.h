// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RallyReceiverComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PLATINUMA3_API URallyReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region Unreal Defaults
public:
	// Sets default values for this component's properties
	URallyReceiverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region Notify
private:
	UPROPERTY()
	bool bIsNotified;

	UPROPERTY()
	FTimerHandle IsNotifiedTimerHandle;
	
	UPROPERTY()
	FVector Destination;
public:
	bool GetIsNotified() const;
	const FVector& GetDestination() const;
	
	void Notify(const FVector& NewDestination);
	void UnNotify();
	void OnNotifyTimeOut();

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void JuicyNotify();
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly)
	void JuicyUnNotify();

private:
	UPROPERTY()
	float RallySpeed;
#pragma endregion 
};
