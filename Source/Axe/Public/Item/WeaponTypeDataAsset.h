// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/Component/InventoryComponent.h"
#include "WeaponTypeDataAsset.generated.h"

class UGameplayAbility;
class UComboDataAsset;
/**
 * 
 */
UCLASS()
class AXE_API UWeaponTypeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAxePlayerWeaponType WeaponType = EAxePlayerWeaponType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> WeaponAnimLayerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UComboDataAsset> WeaponComboDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;
};
