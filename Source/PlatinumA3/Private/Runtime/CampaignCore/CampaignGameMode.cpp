// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Characters/StateCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIGroupSubsystem.h"
#include "Runtime/Camera/SplineCameraScroller.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/CampaignCore/CampaignPlayerStart.h"
#include "Runtime/CampaignCore/GameStateID.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/Sheep/SheepCharacter.h"
#include "Runtime/SheepSystem/SheepSubsystem.h"

#pragma region Defaults
void ACampaignGameMode::BeginPlay()
{
	Super::BeginPlay();
	//Init Game
	GameStateID = EGameStateID::Not_Finishable;
	
	//Spawn Players
	CreateAndInitPlayers();

	TArray<AStateCharacter*> Characters;

	TArray<ACampaignPlayerStart*> PlayerStarts;
	FindPlayerStartsActors(PlayerStarts);
	Characters = SpawnCharacters(PlayerStarts);

	
	//Init Camera
	FindAndInitSplineCamera(Characters);

	//Init GameInstance subsystems
	InitGameInstanceSubsystems();
	
	//Init World subsystems
	InitWorldSubsystems();
	BindToWorldSubsystemsEvents();


	//Setup Game
	// FindAllSheepsInWorld(AllSheeps);	
	// //Reset GameValue
	// SheepSaved = 0;
	// SetNbSheepToSucceedLevel(1);
}

void ACampaignGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnbindToWorldSubsystemsEvents();
}


#pragma endregion

#pragma region Initialization
void ACampaignGameMode::InitWorldSubsystems() const
{
	//Init AIGroupSubsystem
	UAIGroupSubsystem* UAIGroupSubsystem = GetWorld()->GetSubsystem<class UAIGroupSubsystem>();
	if(UAIGroupSubsystem != nullptr)
	{
		UAIGroupSubsystem->InitSubsystem();
	}
	//Init WorldSubsystem working around the AIGroupSubsystem
	UFleeSubsystem* FleeSubsystem = GetWorld()->GetSubsystem<UFleeSubsystem>();
	if(FleeSubsystem != nullptr)
	{
		FleeSubsystem->InitSubsystem();
	}

	//Init Sheep Counting subsystem
	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem != nullptr)
	{
		SheepSubsystem->InitSubsystem(0);
	}
}

void ACampaignGameMode::BindToWorldSubsystemsEvents() const
{
	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem != nullptr)
	{
		SheepSubsystem->ReachedRequiredSheepAmountEvent.AddDynamic(this, &ACampaignGameMode::OnReachedSheepAmountEvent);
	}
}

void ACampaignGameMode::InitGameInstanceSubsystems() const
{
	
}



#pragma endregion 

#pragma region OnEndPlay
void ACampaignGameMode::UnbindToWorldSubsystemsEvents() const
{
	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem != nullptr)
	{
		SheepSubsystem->ReachedRequiredSheepAmountEvent.RemoveDynamic(this, &ACampaignGameMode::OnReachedSheepAmountEvent);
	}
}
#pragma endregion 

#pragma region LocalPlayers
void ACampaignGameMode::FindPlayerStartsActors(TArray<ACampaignPlayerStart*>& InOutPlayerStarts) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampaignPlayerStart::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ACampaignPlayerStart* CampaignPlayerStart = Cast<ACampaignPlayerStart>(FoundActor);

		if (CampaignPlayerStart == nullptr) continue;
		
		InOutPlayerStarts.Add(CampaignPlayerStart);
	}
}

TArray<AStateCharacter*> ACampaignGameMode::SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts) const
{
	
	TArray<AStateCharacter*> Characters;
	
	for (ACampaignPlayerStart* CampaignPlayerStart : InPlayerStarts)
	{
		EAutoReceiveInput::Type InputType = CampaignPlayerStart->AutoReceiveInput.GetValue();
		TSubclassOf<AStateCharacter> CharacterClass = GetCampaignCharacterClassByInputType(InputType);
		if(CharacterClass == nullptr) continue;
		
		AStateCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AStateCharacter>(
			CharacterClass,
			CampaignPlayerStart->GetTransform()
			);

		
		if(NewCharacter == nullptr) continue;

		NewCharacter->AutoPossessPlayer = CampaignPlayerStart->AutoReceiveInput;
		NewCharacter->FinishSpawning(CampaignPlayerStart->GetTransform());

		Characters.Add(NewCharacter);
	}

	return Characters;
}

void ACampaignGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr) return;
	
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(LocalMultiplayerSubsystem == nullptr) return;
	
	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

void ACampaignGameMode::FindAndInitSplineCamera(TArray<AStateCharacter*>& Characters) const
{
	ASplineCameraScroller* SplineCamera = Cast<ASplineCameraScroller>(UGameplayStatics::GetActorOfClass(GetWorld(), ASplineCameraScroller::StaticClass()));
	if(SplineCamera == nullptr) return;
	
	SplineCamera->InitializedSplineCameraScroller(Characters);
}

TSubclassOf<AStateCharacter> ACampaignGameMode::GetCampaignCharacterClassByInputType(
	EAutoReceiveInput::Type InInputType) const
{
	const UCampaignModeSettings* CampaignModeSettings = GetDefault<UCampaignModeSettings>();
	if(CampaignModeSettings == nullptr) return nullptr;

	switch (InInputType)
	{
	case EAutoReceiveInput::Player0:
		return CampaignModeSettings->CampaignCharacterClassP0;
	case EAutoReceiveInput::Player1:
		return CampaignModeSettings->CampaignCharacterClassP1;
	default: return nullptr;
	}
}

#pragma endregion


#pragma region GameLoop


void ACampaignGameMode::FinishGame()
{
	SetGameStateID(EGameStateID::Finished);
	GameFinishedEvent.Broadcast();
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Turquoise,
		TEXT("Finish Game"));
}

EGameStateID ACampaignGameMode::GetGameStateID() const
{
	return GameStateID;
}

void ACampaignGameMode::SetGameStateID(EGameStateID NewGameStateID)
{
	GameStateID = NewGameStateID;
}

void ACampaignGameMode::OnReachedSheepAmountEvent()
{
	SetGameStateID(EGameStateID::Finishable);

	
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("CAN FINISH"));
}
#pragma endregion 