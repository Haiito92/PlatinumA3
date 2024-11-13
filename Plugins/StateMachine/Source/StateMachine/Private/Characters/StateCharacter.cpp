// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StateCharacter.h"

#include "DetailLayoutBuilder.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Characters/CharacterStateMachine.h"
#include "Characters/Datas/CharacterInputData.h"
#include "Characters/PDA/PDA_StateDatas.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"









AStateCharacter::AStateCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, m_RotationRate, 0.0f);


	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void AStateCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->RotationRate = FRotator(0.0f, m_RotationRate, 0.0f);


	// // Ensure Enhanced Input Subsystem is used
	// if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	// {
	// 	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	// 	if (Subsystem)
	// 	{
	// 		Subsystem->AddMappingContext(InputMappingContext, 0);
	// 	}
	// }
	
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

void AStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(!EnhancedInputComponent) return;


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
	
	if(InputData->HoldingAction)
	{
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnHolding);
		EnhancedInputComponent->BindAction(InputData->HoldingAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnHolding);
	}

	if(InputData->LaunchAction)
	{
		EnhancedInputComponent->BindAction(InputData->LaunchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLaunching);
		EnhancedInputComponent->BindAction(InputData->LaunchAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnLaunching);
	}

	if(InputData->RallyAction)
	{
		EnhancedInputComponent->BindAction(InputData->RallyAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRallying);
		EnhancedInputComponent->BindAction(InputData->RallyAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnRallying);
	}
}



// void AStateCharacter::SetUpInputMappingContext() const
// {
// 	APlayerController* PlayerController = Cast<APlayerController>(Controller);
// 	if(!PlayerController) return;
//
// 	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
// 	if(!Player) return;
//
// 	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
// 	if(!InputSystem) return;
//
// 	InputSystem->AddMappingContext(InputMappingContext, 0);
// }

void AStateCharacter::Input_OnMove(const FInputActionValue& ActionValue)
{
	InputMoves = ActionValue.Get<FVector2D>();
}



void AStateCharacter::Input_OnHolding(const FInputActionValue& ActionValue)
{
	IsHolding = ActionValue.Get<bool>();
}

void AStateCharacter::Input_OnLaunching(const FInputActionValue& ActionValue)
{
	IsLaunching = ActionValue.Get<bool>();
}

void AStateCharacter::Input_OnRallying(const FInputActionValue& ActionValue)
{
	IsRallying = ActionValue.Get<bool>();
}



void AStateCharacter::Move(FVector2D MovementsValues)
{



	 // FVector2D MovementVector = MovementsValues;
	 //
	 // if (m_CameraActor != nullptr && m_CameraActor != nullptr)
	 // {
	 // 	// Get the Camera's rotation and isolate the Yaw
	 // 	const FRotator CameraRotation = m_CameraActor->GetActorRotation();
	 // 	const FRotator YawRotation(0, CameraRotation.Yaw, 0);
	 //
	 // 	// Get Forward and Right direction vectors based on camera's yaw rotation
	 // 	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	 // 	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	 //
	 // 	// Add movement inputs relative to the camera direction
	 // 	AddMovementInput(ForwardDirection, MovementVector.Y);
	 // 	AddMovementInput(RightDirection, MovementVector.X);
	 // }



	

	
	// FVector2D MovementVector = MovementsValues;
	// FVector NextPosition;
	//
	// FVector ForwardDirection;
	// FVector RightDirection;
	// if (m_CameraActor != nullptr && m_CameraActor != nullptr)
	// {
	// 	const FRotator CameraRotation = m_CameraActor->GetActorRotation();
	// 	const FRotator YawRotation(0, CameraRotation.Yaw, 0);
	//
	// 	ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 	RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//
	// 	// Add movement inputs relative to the camera direction
	// 	NextPosition = GetActorLocation();
	// 	NextPosition += ForwardDirection * MovementVector.Y * 2.0f;
	// 	NextPosition += RightDirection * MovementVector.X * 2.0f;
	// }
	//
	//
	//
	// APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
 //        
	// if (PlayerController)
	// {
	// 	FVector PlayerWorldPosition = NextPosition;
	// 	FVector2D ScreenPosition;
	//
	// 	bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(PlayerWorldPosition, ScreenPosition);
	//
	// 	if (bIsOnScreen)
	// 	{
	// 		// Get viewport size
	// 		int32 ScreenWidth, ScreenHeight;
	// 		PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);
	//
	// 		float Margin = 0.2f;
	// 		float MinX = ScreenWidth * Margin;
	// 		float MaxX = ScreenWidth * (1.0f - Margin);
	// 		float MinY = ScreenHeight * Margin;
	// 		float MaxY = ScreenHeight * (1.0f - Margin);
	//
	// 		// Check if the position is within the screen boundaries
	// 		if (ScreenPosition.X < MinX || ScreenPosition.X > MaxX || ScreenPosition.Y < MinY || ScreenPosition.Y > MaxY)
	// 		{
	// 			
	// 		}else
	// 		{
	// 			AddMovementInput(ForwardDirection, MovementVector.Y);
	// 			AddMovementInput(RightDirection, MovementVector.X);
	// 		}
	// 	}
	// }



	
	FVector2D MovementVector = MovementsValues;
	FVector NextPosition;

	FVector ForwardDirection;
	FVector RightDirection;

	if (m_CameraActor != nullptr)
	{
	    const FRotator CameraRotation = m_CameraActor->GetActorRotation();
	    const FRotator YawRotation(0, CameraRotation.Yaw, 0);

	    ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	    RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	    // Calculate the next position relative to the camera direction
	    NextPosition = GetActorLocation();
	    NextPosition += ForwardDirection * MovementVector.Y * 2.0f;
	    NextPosition += RightDirection * MovementVector.X * 2.0f;
	}

	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		FVector PlayerWorldPosition = NextPosition;
		FVector2D ScreenPosition;

		bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(PlayerWorldPosition, ScreenPosition);

		if (bIsOnScreen)
		{
			// Get viewport size
			int32 ScreenWidth, ScreenHeight;
			PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

			float Margin = 0.1f; // Margin for movement limit near screen border
			float MinX = ScreenWidth * Margin;
			float MaxX = ScreenWidth * (1.0f - Margin);
			float MinY = ScreenHeight * Margin;
			float MaxY = ScreenHeight * (1.0f - Margin);

			// Adjust movement based on proximity to the screen border
			float XScale = 1.0f;
			float YScale = 1.0f;

			if (ScreenPosition.X < MinX) XScale = FMath::Clamp((ScreenPosition.X - MinX) / MinX, 0.0f, 1.0f);
			else if (ScreenPosition.X > MaxX) XScale = FMath::Clamp((MaxX - ScreenPosition.X) / (ScreenWidth - MaxX), 0.0f, 1.0f);

			if (ScreenPosition.Y < MinY) YScale = FMath::Clamp((ScreenPosition.Y - MinY) / MinY, 0.0f, 1.0f);
			else if (ScreenPosition.Y > MaxY) YScale = FMath::Clamp((MaxY - ScreenPosition.Y) / (ScreenHeight - MaxY), 0.0f, 1.0f);

			// Apply scaled movement input
			AddMovementInput(ForwardDirection, MovementVector.Y * YScale);
			AddMovementInput(RightDirection, MovementVector.X * XScale);

			if (ScreenPosition.X < MinX || ScreenPosition.X > MaxX || ScreenPosition.Y < MinY || ScreenPosition.Y > MaxY)
			{
				// GEngine->AddOnScreenDebugMessage(
				//  -1,
				//  3.0f,
				//  FColor::Cyan,
				//  "Out of screen"
				//  );
			}
		}
	}

	
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

bool AStateCharacter::GetIsHolding()
{
	return IsHolding;
}

bool AStateCharacter::GetIsRallying()
{
	return IsRallying;
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
