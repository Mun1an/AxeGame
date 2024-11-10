// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"

#include "GameplayTagsManager.h"

FAxeGameplayTags FAxeGameplayTags::Instance;

void FAxeGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_LightAttack, "InputTag.LightAttack");
	AddTag(InputTag_HeavyAttack, "InputTag.HeavyAttack");
	AddTag(InputTag_Skill_1, "InputTag.Skill.1");
	AddTag(InputTag_Skill_2, "InputTag.Skill.2");
	AddTag(InputTag_Skill_3, "InputTag.Skill.3");
	AddTag(InputTag_Skill_4, "InputTag.Skill.4");
	AddTag(InputTag_Dash, "InputTag.Dash");

	AddTag(Ability_Attack_LightAttack, "Ability.Attack.LightAttack");
	AddTag(Ability_Attack_HeavyAttack, "Ability.Attack.HeavyAttack");
	AddTag(Ability_Move_Dash, "Ability.Move.Dash");

	AddTag(Ability_Attack_MeleeCombo_A1, "Ability.Attack.MeleeCombo.A1");
	AddTag(Ability_Attack_MeleeCombo_A2, "Ability.Attack.MeleeCombo.A2");
	AddTag(Ability_Attack_MeleeCombo_A3, "Ability.Attack.MeleeCombo.A3");
	AddTag(Ability_Attack_MeleeCombo_A4, "Ability.Attack.MeleeCombo.A4");

	AddTag(Ability_Attack_MeleeCombo_B1, "Ability.Attack.MeleeCombo.B1");
	AddTag(Ability_Attack_MeleeCombo_B2, "Ability.Attack.MeleeCombo.B2");
	AddTag(Ability_Attack_MeleeCombo_B3, "Ability.Attack.MeleeCombo.B3");

	AddTag(Ability_Skill_LeapAttack, "Ability.Skill.LeapAttack");

	AddTag(Ability_HitReact_Light, "Ability.HitReact.Light");
	AddTag(Ability_HitReact_Heavy, "Ability.HitReact.Heavy");

	AddTag(GameplayCue_WeaponTrail_Axe, "GameplayCue.WeaponTrail.Axe");
	AddTag(GameplayCue_WeaponHit_Axe, "GameplayCue.WeaponHit.Axe");
	AddTag(GameplayCue_WeaponHit_Shield, "GameplayCue.WeaponHit.Shield");
	
	//
	AddTag(Attributes_Vital_Health, "Attributes.Vital.Health");
	AddTag(Attributes_Primary_Strength, "Attributes.Primary.Strength");
	AddTag(Attributes_Primary_Dexterity, "Attributes.Primary.Dexterity");
	AddTag(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence");
	AddTag(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth");
	AddTag(Attributes_Secondary_Armor, "Attributes.Secondary.Armor");
	AddTag(Attributes_Secondary_Evasive, "Attributes.Secondary.Evasive");
	AddTag(Attributes_Secondary_PhysicalResistance, "Attributes.Secondary.PhysicalResistance");
	AddTag(Attributes_Secondary_MagicResistance, "Attributes.Secondary.MagicResistance");
	AddTag(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance");
	AddTag(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage");
	AddTag(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration");
	AddTag(Attributes_Secondary_MovementSpeed, "Attributes.Secondary.MovementSpeed");
	AddTag(Attributes_Secondary_BaseDamage, "Attributes.Secondary.BaseDamage");

	AddTag(Damage_Physical, "Damage.Physical");
	AddTag(Damage_Magic, "Damage.Magic");

	DamageTypesToResistances.Add(Damage_Physical, Attributes_Secondary_PhysicalResistance);
	DamageTypesToResistances.Add(Damage_Magic, Attributes_Secondary_MagicResistance);
}


void FAxeGameplayTags::InitNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	Instance.AddAllTags(GameplayTagsManager);
}

void FAxeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	if (TagComment == nullptr)
	{
		TagComment = TagName;
	}

	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TagName),
		FString(TEXT("(Native) ")) + FString(TagComment)
	);
}
