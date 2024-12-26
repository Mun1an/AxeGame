// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_EquipmentInfo.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct FEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentArmor = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentMaxHealth = 0.0f;
};

UCLASS(BlueprintType)
class AXE_API UItemFragment_EquipmentInfo : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_EquipmentInfo()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FEquipmentInfo EquipmentInfo;
};
