// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterStateID.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class UPDA_StateDatas;
class USmashCharacterStateMachine;
struct FInputActionValue;
class UEnhancedInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);


UCLASS()
class STATEMACHINE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Input Data / Mapping Context
	
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetUpInputMappingContext() const;
	
#pragma endregion
	
#pragma region Unreal Default
	
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma endregion



	
#pragma region Orient

public:
float GetOrientX() const;

void SetOrientX(float NewOrientX);


protected:
	UPROPERTY(BlueprintReadWrite)
	float OrientX = 1.0f;

	void RotateMeshUsingOrientX() const;

	
#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ESmashCharacterStateID, TObjectPtr<UPDA_StateDatas>> StatesDatas;

	UPDA_StateDatas* GetStateDatas(ESmashCharacterStateID StateID);
	
#pragma endregion

#pragma region Input Move X


public:
	float GetInputMoveX();
	float GetInputMoveY();

	float GetInputMoveZ();

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
	
protected:
	UPROPERTY()
	float InputMoveX = 0.0f;

	UPROPERTY()
	float InputMoveY = 0.0f;

	
	UPROPERTY()
	float InputMoveZ = 0.0f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);

	void OnInputMoveY(const FInputActionValue& InputActionValue);
	void OnInputMoveYFast(const FInputActionValue& InputActionValue);
	
	void OnInputMoveZ(const FInputActionValue& InputActionValue);


#pragma endregion
};
