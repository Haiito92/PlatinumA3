// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/GameFeel/SmoothRotationComponent.h"

#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"

#pragma region Unreal Defaults
// Sets default values for this component's properties
USmoothRotationComponent::USmoothRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void USmoothRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	if(RotationCurve == nullptr) return;

	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("SetOwnerRotation"));
	SmoothRotateTimeline.AddInterpFloat(RotationCurve, TimelineCallback);

	SmoothRotateTimeline.SetPlayRate(1/RotationTime);
	// ...
	
}


// Called every frame
void USmoothRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SmoothRotateTimeline.TickTimeline(DeltaTime);
	// ...
}

void USmoothRotationComponent::StopSmoothRotate()
{
	SmoothRotateTimeline.Stop();
}

void USmoothRotationComponent::SetOwnerRotation()
{
	// UE_LOGFMT(LogTemp, Warning, "SetOwnerRotation");
	
	const float TimelineValue = SmoothRotateTimeline.GetPlaybackPosition();
	const float CurveFloatValue = RotationCurve->GetFloatValue(TimelineValue);

	const FRotator NewRotation = UKismetMathLibrary::RLerp(
		StartRotation,
		GoalRotation,
		CurveFloatValue,
		true);

	GetOwner()->SetActorRotation(NewRotation);
}


#pragma endregion

#pragma region SmoothRotation

void USmoothRotationComponent::SmoothRotate_Implementation(const FRotator& InGoalRotation)
{
	StartRotation = GetOwner()->GetActorRotation();
	GoalRotation = InGoalRotation;
	
	SmoothRotateTimeline.PlayFromStart();
}

#pragma endregion 