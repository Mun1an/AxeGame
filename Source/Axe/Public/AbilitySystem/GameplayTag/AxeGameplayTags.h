// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

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

	FGameplayTag Ability_Attack_LightAttack;
	FGameplayTag Ability_Attack_HeavyAttack;
	FGameplayTag Ability_Move_Dash;

	FGameplayTag Ability_Attack_MeleeCombo_A1;
	FGameplayTag Ability_Attack_MeleeCombo_A2;
	FGameplayTag Ability_Attack_MeleeCombo_A3;
	FGameplayTag Ability_Attack_MeleeCombo_A4;

	FGameplayTag Ability_Attack_MeleeCombo_B1;
	FGameplayTag Ability_Attack_MeleeCombo_B2;
	FGameplayTag Ability_Attack_MeleeCombo_B3;

	FGameplayTag Ability_Skill_ShieldDefense;
	FGameplayTag Ability_Skill_ShieldParry;
	FGameplayTag Ability_Skill_LeapAttack;

	FGameplayTag Ability_HitReact_Light;
	FGameplayTag Ability_HitReact_Heavy;

	FGameplayTag GameplayCue_WeaponTrail_Axe;
	FGameplayTag GameplayCue_WeaponHit_Axe;
	FGameplayTag GameplayCue_WeaponHit_Shield;
	FGameplayTag GameplayCue_ShieldParry;

	/**
	 * Attributes
	 */
	// Vital
	FGameplayTag Attributes_Vital_Health;
	// Primary
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	// Secondary
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_Evasive;
	FGameplayTag Attributes_Secondary_PhysicalResistance;
	FGameplayTag Attributes_Secondary_MagicResistance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_MovementSpeed;
	FGameplayTag Attributes_Secondary_BaseDamage;

	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magic;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	// State
	FGameplayTag State_HitReact;
	FGameplayTag State_Blocking;
	FGameplayTag State_PrepareParry;

protected:
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName = nullptr, const ANSICHAR* TagComment = nullptr);
	void AddAllTags(UGameplayTagsManager& Manager);

private:
	static FAxeGameplayTags Instance;
};
