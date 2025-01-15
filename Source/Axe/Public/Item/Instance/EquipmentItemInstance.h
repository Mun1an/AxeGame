// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "EquipmentItemDefinition.h"
#include "ItemInstance.h"
#include "Engine/World.h"

#include "EquipmentItemInstance.generated.h"

class UAbilitySystemComponent;
struct FAxeEquipmentActorToSpawn;
class AActor;
class APawn;

UENUM(Blueprintable, BlueprintType)
enum class EEquipmentRarity : uint8
{
	Common = 0,
	Uncommon = 1,
	Rare = 2,
	Epic = 3,
	Legendary = 4,
	Max = 5
};

USTRUCT(BlueprintType)
struct FEquipmentInstanceAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FGameplayTag AttributeTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float AttributeValue = 0.0f;
};

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class UEquipmentItemInstance : public UItemInstance
{
	GENERATED_BODY()

public:
	UEquipmentItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	virtual void SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();

	void InitEquipmentItemInstanceInfo(int32 ItemLevelValue,
	                                   EEquipmentRarity ItemRarity = EEquipmentRarity::Common);

	virtual void OnItemInstanceCreated() override;

	virtual void CreateItemDescription() override;

protected:
	// #if UE_WITH_IRIS
	// 	/** Register all replication fragments */
	// 	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
	// #endif // UE_WITH_IRIS

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY()
	int32 EquipmentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	EEquipmentRarity EquipmentRarity = EEquipmentRarity::Common;
	
	// Equipment
	UPROPERTY(BlueprintReadOnly, Category="Equipment|Attr")
	TArray<FEquipmentInstanceAttributeInfo> EquipmentInstanceAttributeInfos;

private:
	FActiveGameplayEffectHandle EquipmentEffectHandle;
};
