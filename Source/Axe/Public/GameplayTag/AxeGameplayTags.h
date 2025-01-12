// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

enum class EAxePlayerWeaponType : uint8;
/**
 * 
 */
struct FAxeGameplayTags
{
	static void InitNativeGameplayTags();

	static FAxeGameplayTags& Get()
	{
		return Instance;
	}

	FGameplayTag InputTag_LightAttack;
	FGameplayTag InputTag_HeavyAttack;
	FGameplayTag InputTag_Skill_1;
	FGameplayTag InputTag_Skill_2;
	FGameplayTag InputTag_Skill_3;
	FGameplayTag InputTag_Skill_4;
	FGameplayTag InputTag_Dash;
	FGameplayTag InputTag_Defense;
	FGameplayTag InputTag_Interact;
	FGameplayTag InputTag_Jump;

	FGameplayTag Ability_Attack_LightAttack;
	FGameplayTag Ability_Attack_HeavyAttack;
	FGameplayTag Ability_Move_Dash;
	FGameplayTag Ability_Move_Jump;

	FGameplayTag Ability_Attack_MeleeCombo_A1;
	FGameplayTag Ability_Attack_MeleeCombo_A2;
	FGameplayTag Ability_Attack_MeleeCombo_A3;
	FGameplayTag Ability_Attack_MeleeCombo_A4;

	FGameplayTag Ability_Attack_MeleeCombo_B1;
	FGameplayTag Ability_Attack_MeleeCombo_B2;
	FGameplayTag Ability_Attack_MeleeCombo_B3;

	FGameplayTag Ability_Skill_Defense_Block;
	FGameplayTag Ability_Skill_Defense_Parry;
	FGameplayTag Ability_Skill_Defense_Stagger;
	
	FGameplayTag Ability_Skill_LeapAttack;
	FGameplayTag Ability_Skill_ThrowAxe;
	FGameplayTag Ability_Skill_Roar;

	FGameplayTag Ability_HitReact_Light;
	FGameplayTag Ability_HitReact_Heavy;
	//
	FGameplayTag GameplayCue_WeaponWave_Axe;
	FGameplayTag GameplayCue_WeaponHit_Axe;
	FGameplayTag GameplayCue_WeaponHit_Shield;
	FGameplayTag GameplayCue_ShieldParry;
	FGameplayTag GameplayCue_GroundSmash;

	// Cooldown
	FGameplayTag Cooldown_Skill_LeapAttack;
	FGameplayTag Cooldown_Skill_ThrowAxe;
	FGameplayTag Cooldown_Skill_Roar;

	/**
	 * Attributes
	 */
	// Vital
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Stamina;
	// Primary
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	// Secondary
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_Evasive;
	FGameplayTag Attributes_Secondary_PhysicalResistance;
	FGameplayTag Attributes_Secondary_MagicResistance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;
	FGameplayTag Attributes_Secondary_MovementSpeed;
	FGameplayTag Attributes_Secondary_BaseDamage;

	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magic;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	TArray<FGameplayTag> AllAttributesList;
	TArray<FGameplayTag> AllPrimaryTagList;
	TArray<FGameplayTag> AllSecondaryTagList;

	// State
	FGameplayTag State_HitReact;
	FGameplayTag State_Blocking;
	FGameplayTag State_PrepareParry;

	// Effect
	FGameplayTag Effect_Asset_Damage;
	// Effect_Magnitude
	FGameplayTag Effect_Magnitude_1;
	FGameplayTag Effect_Magnitude_Damage;
	FGameplayTag Effect_Magnitude_Stamina;
	FGameplayTag Effect_Magnitude_Armor;
	FGameplayTag Effect_Magnitude_MaxHealth;
	FGameplayTag Effect_Magnitude_IncomingXp;

	/**
	 * Item
	 */
	FGameplayTag Inventory_Entry_Bag;
	FGameplayTag Inventory_Entry_Equipment;
	FGameplayTag Inventory_Entry_Equipment_Armor_Helmet;
	FGameplayTag Inventory_Entry_Equipment_Armor_Chestplate;
	FGameplayTag Inventory_Entry_Equipment_Armor_Leggings;
	FGameplayTag Inventory_Entry_Equipment_Armor_Boots;
	FGameplayTag Inventory_Entry_Equipment_Weapon;

	FGameplayTag ItemType_None;
	FGameplayTag ItemType_Food;
	FGameplayTag ItemType_Equipment_Armor_Helmet;
	FGameplayTag ItemType_Equipment_Armor_Chestplate;
	FGameplayTag ItemType_Equipment_Armor_Leggings;
	FGameplayTag ItemType_Equipment_Armor_Boots;
	FGameplayTag ItemType_Equipment_Weapon;
	FGameplayTag ItemType_Equipment_Weapon_AxeAndShield;
	FGameplayTag ItemType_Equipment_Weapon_GreatAxe;
	FGameplayTag ItemType_Equipment_Weapon_DualAxe;

	TMap<FGameplayTag, EAxePlayerWeaponType> TagWeaponTypeMap;

	/**
	 * Interact
	 */
	FGameplayTag Ability_Interaction_Activate;

public:
	EAxePlayerWeaponType GetWeaponTypeByTag(const FGameplayTag& Tag) const;

protected:
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName = nullptr, const ANSICHAR* TagComment = nullptr,
	            bool bIsAttributesTag = false);
	void AddAllTags(UGameplayTagsManager& Manager);

	void OnTagsAdded();

private:
	static FAxeGameplayTags Instance;
};
