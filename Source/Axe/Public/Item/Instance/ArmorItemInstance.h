// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EquipmentItemInstance.h"
#include "Engine/World.h"

#include "ArmorItemInstance.generated.h"

class UModularCharacterComponent;
struct FAxeEquipmentActorToSpawn;
class AActor;
class APawn;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class UArmorItemInstance : public UEquipmentItemInstance
{
	GENERATED_BODY()

public:
	UArmorItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn) override;
	virtual void DestroyEquipmentActors() override;

	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

protected:
	UFUNCTION()
	UModularCharacterComponent* GetModularCharacterComponent();
	UPROPERTY()
	UModularCharacterComponent* ModularCharacterComponent = nullptr;
};
