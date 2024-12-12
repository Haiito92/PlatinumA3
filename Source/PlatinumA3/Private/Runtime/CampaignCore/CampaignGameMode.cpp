// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/CampaignCore/CampaignGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Blueprints/FCTweenBlueprintLibrary.h"
#include "Characters/StateCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIGroupSystem/AIGroupSubsystem.h"
#include "Runtime/Camera/SplineCameraScroller.h"
#include "Runtime/CampaignCore/CampaignModeSettings.h"
#include "Runtime/CampaignCore/CampaignPlayerStart.h"
#include "Runtime/CampaignCore/CampaignGameStateID.h"
#include "Runtime/CampaignCore/CampaignHUD.h"
#include "Runtime/CampaignCore/EWoolStateClassID.h"
#include "Runtime/Characters/WoolStateCharacter.h"
#include "Runtime/CheatsSystem/CheatsSubsystem.h"
#include "Runtime/FleeSystem/FleeSubsystem.h"
#include "Runtime/SheepSystem/SheepSubsystem.h"

#pragma region Defaults
void ACampaignGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UFCTweenBlueprintLibrary::EnsureTweenCapacity(150,150);
	
	//Init Game
	GameStateID = ECampaignGameStateID::Not_Finishable;
	
	//Spawn Players
	CreateAndInitPlayers();

	TArray<ACampaignPlayerStart*> PlayerStarts;
	FindPlayerStartsActors(PlayerStarts);
	TArray<AStateCharacter*> Characters;
	SpawnCharacters(PlayerStarts, Characters);

	
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
		SheepSubsystem->InitSubsystem(3);
	}

// #if UE_EDITOR
	//Init CheatSubsystem
	UCheatsSubsystem* CheatsSubsystem = GetWorld()->GetSubsystem<UCheatsSubsystem>();
	if(CheatsSubsystem != nullptr)
	{
		CheatsSubsystem->InitSubsystem();
	}
	
// #endif
}

void ACampaignGameMode::BindToWorldSubsystemsEvents() const
{
	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem != nullptr)
	{
		SheepSubsystem->ReachedRequiredSheepAmountEvent.AddDynamic(this, &ACampaignGameMode::OnReachedSheepAmountEvent);
		SheepSubsystem->NotEnoughSheepLeftEvent.AddDynamic(this, &ACampaignGameMode::OnNotEnoughSheepLeftEvent);
	}
}

void ACampaignGameMode::InitGameInstanceSubsystems() const
{
	
}

// void ACampaignGameMode::InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer)
// {
// 	Super::InitializeHUDForPlayer_Implementation(NewPlayer);
//
// 	if(NewPlayer->GetLocalPlayer()->GetIndexInGameInstance() != 0) return;
//
// 	ACampaignHUD* CampaignHUD = Cast<ACampaignHUD>(NewPlayer->GetHUD());
// 	if(CampaignHUD == nullptr) return;
//
// 	
//
// }


#pragma endregion 

#pragma region OnEndPlay
void ACampaignGameMode::UnbindToWorldSubsystemsEvents() const
{
	USheepSubsystem* SheepSubsystem = GetWorld()->GetSubsystem<USheepSubsystem>();
	if(SheepSubsystem != nullptr)
	{
		SheepSubsystem->ReachedRequiredSheepAmountEvent.RemoveDynamic(this, &ACampaignGameMode::OnReachedSheepAmountEvent);
		SheepSubsystem->NotEnoughSheepLeftEvent.RemoveDynamic(this, &ACampaignGameMode::OnNotEnoughSheepLeftEvent);
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

void ACampaignGameMode::SpawnCharacters(TArray<ACampaignPlayerStart*>& InPlayerStarts, TArray<AStateCharacter*>& InOutCharacters) const
{
	InOutCharacters.Empty();
	
	for (ACampaignPlayerStart* CampaignPlayerStart : InPlayerStarts)
	{
		EAutoReceiveInput::Type InputType = GetPlayerByCharacterClassID(CampaignPlayerStart->GetWoolStateClassID());
		if(InputType == EAutoReceiveInput::Disabled) return;
		CampaignPlayerStart->AutoReceiveInput=InputType;
		
		// EAutoReceiveInput::Type InputType = CampaignPlayerStart->AutoReceiveInput.GetValue();
		// TSoftClassPtr<AStateCharacter> SoftCharacterClass = GetCampaignCharacterClassByInputType(InputType);
		// UClass* CharacterClass = SoftCharacterClass.LoadSynchronous();
		TSubclassOf<AWoolStateCharacter> CharacterClass = CampaignPlayerStart->GetClassToSpawn();
		if(CharacterClass == nullptr) continue;
		
		AStateCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AStateCharacter>(
			CharacterClass,
			CampaignPlayerStart->GetTransform()
			);

		
		if(NewCharacter == nullptr) continue;

		NewCharacter->AutoPossessPlayer = CampaignPlayerStart->AutoReceiveInput;
		NewCharacter->FinishSpawning(CampaignPlayerStart->GetTransform());

		InOutCharacters.Add(NewCharacter);
	}
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

TSoftClassPtr<AStateCharacter> ACampaignGameMode::GetCampaignCharacterClassByInputType(
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

TEnumAsByte<EAutoReceiveInput::Type> ACampaignGameMode::GetPlayerByCharacterClassID(EWoolStateClassID InClassID) const
{
	const UCampaignModeSettings* CampaignModeSettings = GetDefault<UCampaignModeSettings>();
	if(CampaignModeSettings == nullptr) return EAutoReceiveInput::Disabled;
	
	switch (InClassID)
	{
	case EWoolStateClassID::Shepherd:
		return CampaignModeSettings->PlayerPlayingShepherd;
	case EWoolStateClassID::Dog:
		return CampaignModeSettings->PlayerPlayingDog;
	default:
			return EAutoReceiveInput::Disabled;
	}
}


#pragma endregion


#pragma region GameLoop


void ACampaignGameMode::FinishGame(bool bInWon)
{
	SetGameStateID(ECampaignGameStateID::Finished);
	GameFinishedEvent.Broadcast(bInWon);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Turquoise,
		TEXT("Finish Game"));
}

ECampaignGameStateID ACampaignGameMode::GetGameStateID() const
{
	return GameStateID;
}

void ACampaignGameMode::SetGameStateID(ECampaignGameStateID NewGameStateID)
{
	GameStateID = NewGameStateID;
}

void ACampaignGameMode::OnNotEnoughSheepLeftEvent()
{
	FinishGame(false);
}

void ACampaignGameMode::OnReachedSheepAmountEvent()
{
	SetGameStateID(ECampaignGameStateID::Finishable);

	
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("CAN FINISH"));
}
#pragma endregion 