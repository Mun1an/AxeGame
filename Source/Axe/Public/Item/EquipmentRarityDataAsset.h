// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentRarityDataAsset.generated.h"

enum class EEquipmentRarity : uint8;
class UGameplayAbility;
class UComboDataAsset;

USTRUCT(BlueprintType)
struct FEquipmentRarityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentRarity EquipmentRarity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RarityName = FName("");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor RarityColor = FColor::White;

	static FEquipmentRarityInfo Empty;
};

/**
 * 
 */
UCLASS()
class AXE_API UEquipmentRarityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEquipmentRarityInfo> EquipmentRarityInfos;

	const FEquipmentRarityInfo& GetEquipmentRarityInfo(EEquipmentRarity EquipmentRarity) const;
};
