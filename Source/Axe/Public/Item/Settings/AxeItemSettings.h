// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AxeItemSettings.generated.h"

class UEquipmentRarityDataAsset;
class UDataTable;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class AXE_API UAxeItemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UAxeItemSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category=Item)
	TSoftObjectPtr<UEquipmentRarityDataAsset> EquipmentRarityDataAsset;
};
