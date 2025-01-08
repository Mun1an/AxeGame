// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemDefinition.h"
#include "Inventory/Component/InventoryComponent.h"
#include "WeaponItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EAxePlayerWeaponType: uint8
{
	None,
	AxeAndShield,
	GreatAxe,
	DualAxe,
};
class UWeaponDataAsset;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class AXE_API UWeaponItemDefinition : public UEquipmentItemDefinition
{
	GENERATED_BODY()

public:
	UWeaponItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAxePlayerWeaponType WeaponType = EAxePlayerWeaponType::None;
	
	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TObjectPtr<UWeaponDataAsset> WeaponDataAsset;
};
