// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/SimpleGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


void ASimpleGameMode::BeginPlay()
{
	Super::BeginPlay();


}


APlayerStart* ASimpleGameMode::FindPlayerStartActorInArena()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		APlayerStart* PlayerStartActor = Cast<APlayerStart>(FoundActors[i]);

		if (PlayerStartActor != nullptr) return PlayerStartActor;
	}
	return nullptr;
}
