// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "AxeGameData.generated.h"

class UGameplayEffect;
class UObject;

/**
 * UAxeGameData
 *
 *	Non-mutable data asset that contains global game data.
 */
UCLASS(BlueprintType, Const,
	Meta = (DisplayName = "Axe Game Data", ShortTooltip = "Data asset containing global game data."))
class UAxeGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAxeGameData();

	// Returns the loaded game data.
	static const UAxeGameData& Get();

public:
	// Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_IncomingXp_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_IncomingGoldCoinCount_SetByCaller;
};
