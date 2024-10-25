// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StateCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Characters/CharacterStateMachine.h"
#include "Characters/Datas/CharacterInputData.h"
#include "Characters/PDA/PDA_StateDatas.h"
#include "Kismet/GameplayStatics.h"









// Sets default values
AStateCharacter::AStateCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStateCharacter::BeginPlay()
{
	Super::BeginPlay();


	// Ensure Enhanced Input Subsystem is used
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	CreateStateMachine();
	InitStateMachine();
}

// Called every frame
void AStateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void AStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//SetUpInputMappingContext();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(!EnhancedInputComponent) return;

	//BindInputMoveXAxisAndActions(EnhancedInputComponent);

	if(!InputData) return;

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Blue,
		TEXT("Input Data Is Valid !")
	);
	
	if(InputData->MoveAction)
	{
		EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnMove);
		EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnMove);
	}
	
	if(InputData->SprintAction)
	{
		EnhancedInputComponent->BindAction(InputData->SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInputComponent->BindAction(InputData->SprintAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnSprint);
	}
	
	if(InputData->HoldingAction)
	{
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnSprint);
	}
}



void AStateCharacter::SetUpInputMappingContext() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(!InputSystem) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

void AStateCharacter::Input_OnMove(const FInputActionValue& ActionValue)
{
	InputMoves = ActionValue.Get<FVector2D>();
}

void AStateCharacter::Input_OnSprint(const FInputActionValue& ActionValue)
{
	IsSprinting = ActionValue.Get<bool>();
}

void AStateCharacter::Input_OnHolding(const FInputActionValue& ActionValue)
{
	IsHolding = ActionValue.Get<bool>();
}






float AStateCharacter::GetOrientX() const
{
	return OrientX;
}

void AStateCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void AStateCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	Rotation.Yaw = -90.0f * OrientX;

	GetMesh()->SetRelativeRotation(Rotation);
}

FVector2D AStateCharacter::GetInputMoves()
{
	return InputMoves;
}

bool AStateCharacter::GetIsSprinting()
{
	return IsSprinting;
}

bool AStateCharacter::GetIsHolding()
{
	return IsHolding;
}

















void AStateCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UCharacterStateMachine>(this);
}

void AStateCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;

	StateMachine->Init(this);
}

void AStateCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

UPDA_StateDatas* AStateCharacter::GetStateDatas(ECharacterStateID StateID)
{
	if(StatesDatas.Contains(StateID))
	{
		return Cast<UPDA_StateDatas>(*StatesDatas.Find(StateID));
	}else
	{
		return nullptr;
	}
}



void AStateCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(!InputData) return;

	if(InputData->MoveAction)
	{
		EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInputComponent->BindAction(InputData->MoveAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnMove);
	}
	
	if(InputData->SprintAction)
	{
		EnhancedInputComponent->BindAction(InputData->SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInputComponent->BindAction(InputData->SprintAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnSprint);
	}


	if(InputData->HoldingAction)
	{
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnSprint);
	}
}


