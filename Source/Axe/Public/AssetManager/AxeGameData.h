// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "AxeGameData.generated.h"

class AAxeWorldItemActor;
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
	/**
	 * Class
	 */
	// Gameplay effect
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_AddAttr_Duration_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_IncomingXp_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_IncomingGoldCoinCount_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> GE_ToughnessRecoverStop;
	
	//
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSoftClassPtr<AAxeWorldItemActor> WorldItemActorClass;

	/**
	 * DataAsset
	 */
	// DataAsset
	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	TSoftObjectPtr<UDataAsset> DA_AttributeUIInfo;
};
