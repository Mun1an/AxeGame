// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EquipmentItemInstance.h"
#include "ItemInstance.h"
#include "Engine/World.h"

#include "WeaponItemInstance.generated.h"

class UAbilitySystemComponent;
struct FAxeEquipmentActorToSpawn;
class AActor;
class APawn;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class UWeaponItemInstance : public UEquipmentItemInstance
{
	GENERATED_BODY()

public:
	UWeaponItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn) override;
	virtual void DestroyEquipmentActors() override;
	
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

	
protected:
	UAbilitySystemComponent* GetAbilitySystemComponent();
	
};
