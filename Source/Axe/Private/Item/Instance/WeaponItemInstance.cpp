// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Instance/WeaponItemInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "Item/WeaponDataAsset.h"
#include "Item/Instance/WeaponItemDefinition.h"
#include "Item/ItemActor/WeaponEquipmentItemActor.h"

UWeaponItemInstance::UWeaponItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UWeaponItemInstance::SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn)
{
	Super::SpawnEquipmentActors(ActorsToSpawn);
}

void UWeaponItemInstance::DestroyEquipmentActors()
{
	Super::DestroyEquipmentActors();
}

void UWeaponItemInstance::OnEquipped()
{
	Super::OnEquipped();

	// Set the weapon equipment actor
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	if (AxeCharacterPlayer)
	{
		if (SpawnedActors.Num() > 0)
		{
			AActor* Actor = SpawnedActors[0];
			AWeaponEquipmentItemActor* WeaponEquipmentItemActor = Cast<AWeaponEquipmentItemActor>(Actor);
			AxeCharacterPlayer->SetWeaponEquipmentActor(WeaponEquipmentItemActor);
		}
		if (SpawnedActors.Num() > 1)
		{
			AActor* Actor = SpawnedActors[1];
			AWeaponEquipmentItemActor* WeaponEquipmentItemActor = Cast<AWeaponEquipmentItemActor>(Actor);
			AxeCharacterPlayer->SetWeaponEquipmentActorSecondary(WeaponEquipmentItemActor);
		}
	}

	// Add
	UWeaponItemDefinition* WeaponItemDefinition = GetItemDef()->GetDefaultObject<UWeaponItemDefinition>();
	UWeaponDataAsset* WeaponDataAsset = WeaponItemDefinition->WeaponDataAsset;

	// Add New Weapon Abilities
	if (AxeCharacterPlayer->HasAuthority() && WeaponDataAsset->WeaponAbilities.Num() > 0)
	{
		for (const TSubclassOf<UGameplayAbility>& Ability : WeaponDataAsset->WeaponAbilities)
		{
			AxeASC->GiveAbilityByAbilityAndLevel(Ability, 1);
		}
	}
	// Add WeaponAnimLayer
	if (WeaponDataAsset->WeaponAnimLayerClass)
	{
		AxeCharacterPlayer->SetLinkedAnimLayerClass(WeaponDataAsset->WeaponAnimLayerClass);
	}
	// Add WeaponCombo
	if (WeaponDataAsset->WeaponComboDataAsset)
	{
		AxeCharacterPlayer->GetComboActionComponent_Implementation()->InitComboAbilityTree(WeaponDataAsset->WeaponComboDataAsset);
	}
}

void UWeaponItemInstance::OnUnequipped()
{
	Super::OnUnequipped();

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	// Set the weapon equipment actor
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->SetWeaponEquipmentActor(nullptr);
		AxeCharacterPlayer->SetWeaponEquipmentActorSecondary(nullptr);
	}

	// Remove
	UWeaponItemDefinition* WeaponItemDefinition = GetItemDef()->GetDefaultObject<UWeaponItemDefinition>();
	UWeaponDataAsset* WeaponDataAsset = WeaponItemDefinition->WeaponDataAsset;

	// Remove Old Weapon Abilities
	if (AxeCharacterPlayer->HasAuthority() && WeaponDataAsset->WeaponAbilities.Num() > 0)
	{
		for (const TSubclassOf<UGameplayAbility>& Ability : WeaponDataAsset->WeaponAbilities)
		{
			FGameplayTag AbilityGameplayTag = Ability.GetDefaultObject()->AbilityTags.First();
			FGameplayAbilitySpecHandle Handle = AxeASC->GetAbilityHandleByAbilityTag(AbilityGameplayTag);
			AxeASC->ClearAbility(Handle);
		}
	}

	if (WeaponDataAsset->WeaponAnimLayerClass)
	{
		AxeCharacterPlayer->ResetLinkedAnimLayerClass();
	}

	if (WeaponDataAsset->WeaponComboDataAsset)
	{
		AxeCharacterPlayer->GetComboActionComponent_Implementation()->ResetComboAbilityTree();
	}
}

