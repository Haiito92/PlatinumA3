// Copyright 2023 Dmitry Karpukhin. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SkyCreatorWeatherPresetFactory.generated.h"

/**
 * 
 */
UCLASS()
class SKYCREATOREDITORPLUGIN_API USkyCreatorWeatherPresetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	USkyCreatorWeatherPresetFactory();

	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;
	// End UFactory Interface
};
