// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StateCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Characters/CharacterStateMachine.h"
#include "Characters/Datas/CharacterInputData.h"
#include "Characters/PDA/PDA_StateDatas.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"









// Sets default values
AStateCharacter::AStateCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 225.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

// Called when the game starts or when spawned
void AStateCharacter::BeginPlay()
{
	Super::BeginPlay();


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






void AStateCharacter::Move(FVector2D MovementsValues)
{
	// input is a Vector2D
	FVector2D MovementVector = MovementsValues;

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
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

bool AStateCharacter::GetIsSprinting()
{
	return IsSprinting;
}

bool AStateCharacter::GetIsHolding()
{
	return IsHolding;
}


AActor* AStateCharacter::GetSomethingToHold()
{
	// FVector StartLocation = GetActorLocation();
	// FVector ForwardVector = GetActorForwardVector();
	// float CastDistance = 1000.f;
	// FVector EndLocation = StartLocation + (ForwardVector * CastDistance);
	// float SphereRadius = 50.f; 
	//
	// FCollisionQueryParams QueryParams;
	// QueryParams.AddIgnoredActor(this);
	//
	// TArray<FHitResult> HitResults;
	// bool bHit = GetWorld()->SweepMultiByChannel(
	// 	HitResults,
	// 	StartLocation,
	// 	EndLocation,
	// 	FQuat::Identity,
	// 	ECC_Visibility,
	// 	FCollisionShape::MakeSphere(SphereRadius),
	// 	QueryParams
	// );
	//
	// if (bHit)
	// {
	// 	for (auto& Hit : HitResults)
	// 	{
	//
	// 		
	// 		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, SphereRadius, 12, FColor::Blue, false, 1.0f);
	// 		DrawDebugLine(GetWorld(), StartLocation, Hit.ImpactPoint, FColor::Green, false, 1.0f);
	//
	// 		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
	// 	
	// 		return Hit.GetActor();
	// 		
	// 		
	// 	}
	// }else
	// {
	// 	DrawDebugSphere(GetWorld(), EndLocation, SphereRadius, 12, FColor::Blue, false, 1.0f);
	// 	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1.0f);
	// }
	//
	// return nullptr;







	FVector Center = GetActorLocation();
    float Radius = 400.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	if (bOverlap)
	{
		DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Red, false, 1.0f);

		for (auto& Result : OverlapResults)
		{
			if (AActor* OverlappedActor = Result.GetActor())
			{
				
				UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *OverlappedActor->GetName());
			}
		}
	}

	return nullptr;

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


