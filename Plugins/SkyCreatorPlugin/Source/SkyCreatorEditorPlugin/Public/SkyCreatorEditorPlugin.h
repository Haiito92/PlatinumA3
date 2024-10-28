// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateStyle.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "SkyCreatorActor.h"

class FSkyCreatorEditorPluginModule : public IModuleInterface
{
public:

	TSharedPtr<FSlateStyleSet> StyleSet;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};