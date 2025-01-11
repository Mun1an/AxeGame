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


USTRUCT(Blueprintable)
struct FEquipmentItemInstanceInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 EquipmentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	EEquipmentRarity EquipmentRarity = EEquipmentRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentArmor = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentMaxHealth = 0.0f;

	void CalDamage(float DefaultValue)
	{
		EquipmentDamage = DefaultValue * (1 + EquipmentLevel * 0.2f);
		EquipmentDamage += FMath::RoundToFloat(EquipmentDamage * 0.1f) * FMath::RandRange(-1, 1);
		EquipmentDamage = FMath::Max(0.0f, EquipmentDamage);
	}

	void CalArmor(float DefaultValue)
	{
		EquipmentArmor = DefaultValue * (1 + EquipmentLevel * 0.2f);
		EquipmentArmor += FMath::RoundToFloat(EquipmentArmor * 0.1f) * FMath::RandRange(-1, 1);
		EquipmentArmor = FMath::Max(0.0f, EquipmentArmor);
	}

	void CalMaxHealth(float DefaultValue)
	{
		EquipmentMaxHealth = DefaultValue * (1 + EquipmentLevel * 0.2f);
		EquipmentMaxHealth += FMath::RoundToFloat(EquipmentMaxHealth * 0.1f) * FMath::RandRange(-1, 1);
		EquipmentMaxHealth = FMath::Max(0.0f, EquipmentMaxHealth);
	}
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

	FEquipmentItemInstanceInfo& GetEquipmentItemInstanceInfo();
	void SetEquipmentItemInstanceInfo(const FEquipmentItemInstanceInfo& InEquipmentItemInstanceInfo);

	void InitEquipmentItemInstanceInfo(int32 EquipmentLevel,
	                                   EEquipmentRarity EquipmentRarity = EEquipmentRarity::Common);

	virtual void OnItemInstanceCreated() override;

	virtual FString GetItemDescription() override;

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

	// Equipment
	UPROPERTY(Replicated, BlueprintReadWrite, Category=EquipmentInstance)
	FEquipmentItemInstanceInfo EquipmentItemInstanceInfo;

private:
	FActiveGameplayEffectHandle EquipmentEffectHandle;
};
