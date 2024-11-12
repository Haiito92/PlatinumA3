// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/FleeSystem/FleeSubsystem.h"

void UFleeSubsystem::InitSubsystem()
{
	// GEngine->AddOnScreenDebugMessage(
	// 	-1,
	// 	3.0f,
	// 	FColor::Purple,
	// 	TEXT("Init FLEE Subsystem"));
}

// const FVector& FFleeLeaderData::GetLeaderLocation() const
// {
// 	return LeaderLocation;
// }
//
// const FVector& FFleeLeaderData::GetLeaderForwardVector() const
// {
// 	return LeaderForwardVector;
// }
TMap<int, UFleeLeaderComponent*>& UFleeSubsystem::GetCurrentFleeLeaders()
{
	return CurrentFleeLeaders;
}

