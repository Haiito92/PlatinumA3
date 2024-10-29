// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateID.h"
#include "GameFramework/Character.h"
#include "StateCharacter.generated.h"

class UInputMappingContext;
class UCharacterInputData;
class UPDA_StateDatas;
class UCharacterStateMachine;
struct FInputActionValue;
class UEnhancedInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);


UCLASS()
class STATEMACHINE_API AStateCharacter : public ACharacter
{
	GENERATED_BODY()







	
#pragma region Unreal Default
	
public:
	// Sets default values for this character's properties
	AStateCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma endregion



#pragma region Input Data / Mapping Context
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
	TObjectPtr<UCharacterInputData> InputData;

protected:
	void SetUpInputMappingContext() const;

private:
	virtual void Input_OnMove(const FInputActionValue& ActionValue);

	virtual void Input_OnSprint(const FInputActionValue& ActionValue);

	virtual void Input_OnHolding(const FInputActionValue& ActionValue);
	
	virtual void Input_OnLaunching(const FInputActionValue& ActionValue);


#pragma endregion





	

	
#pragma region Orient

public:

	void Move(FVector2D MovementsValues);

float GetOrientX() const;

void SetOrientX(float NewOrientX);


protected:
	UPROPERTY(BlueprintReadWrite)
	float OrientX = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float OrientY = 1.0f;
	
	void RotateMeshUsingOrientX() const;

	
#pragma endregion



#pragma region Inputs


public:
	UFUNCTION(BlueprintCallable)
	FVector2D GetInputMoves();

	UFUNCTION(BlueprintCallable)
	bool GetIsSprinting();

	UFUNCTION(BlueprintCallable)
	bool GetIsHolding();
	
protected:
	UPROPERTY()
	FVector2D InputMoves;

	UPROPERTY()
	bool IsSprinting;

	UPROPERTY()
	bool IsHolding;

	UPROPERTY()
	bool IsLaunching;
	
private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);


#pragma endregion








	
#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterStateMachine> StateMachine;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECharacterStateID, TObjectPtr<UPDA_StateDatas>> StatesDatas;

	UPDA_StateDatas* GetStateDatas(ECharacterStateID StateID);
	
#pragma endregion


};
