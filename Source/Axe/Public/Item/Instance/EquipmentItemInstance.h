// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "ItemInstance.h"
#include "Engine/World.h"

#include "EquipmentItemInstance.generated.h"

class UAbilitySystemComponent;
struct FAxeEquipmentActorToSpawn;
class AActor;
class APawn;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class UEquipmentItemInstance : public UItemInstance
{
	GENERATED_BODY()

public:
	UEquipmentItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	virtual void SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
	// #if UE_WITH_IRIS
	// 	/** Register all replication fragments */
	// 	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
	// #endif // UE_WITH_IRIS

	UAbilitySystemComponent* GetAbilitySystemComponent();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

private:
	FActiveGameplayEffectHandle EquipmentEffectHandle;
};
