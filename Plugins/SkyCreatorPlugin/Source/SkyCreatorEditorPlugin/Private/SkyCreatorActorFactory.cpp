// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#include "SkyCreatorActorFactory.h"
#include "SkyCreatorActor.h"

#define LOCTEXT_NAMESPACE "ActorFactory"

USkyCreatorActorFactory::USkyCreatorActorFactory(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    DisplayName = LOCTEXT("SkyCreatorDisplayName", "Sky Creator");
    NewActorClass = ASkyCreator::StaticClass();
}
