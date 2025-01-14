// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTag/AxeGameplayTags.h"

#include "GameplayTagsManager.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Item/Instance/WeaponItemDefinition.h"

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
	AddTag(InputTag_Defense, "InputTag.Defense");
	AddTag(InputTag_Interact, "InputTag.Interact");
	AddTag(InputTag_Jump, "InputTag.Jump");
	AddTag(InputTag_Action_UseItem_1, "InputTag.Action.UseItem.1");
	AddTag(InputTag_Action_UseItem_2, "InputTag.Action.UseItem.2");
	AddTag(InputTag_Action_UseItem_3, "InputTag.Action.UseItem.3");
	AddTag(InputTag_Action_UseItem_4, "InputTag.Action.UseItem.4");

	AddTag(Ability_Attack_LightAttack, "Ability.Attack.LightAttack");
	AddTag(Ability_Attack_HeavyAttack, "Ability.Attack.HeavyAttack");
	AddTag(Ability_Move_Dash, "Ability.Move.Dash");
	AddTag(Ability_Move_Jump, "Ability.Move.Jump");
	
	AddTag(Ability_Action_UseItem, "Ability.Action.UseItem");
	AddTag(Ability_Action_UseItem_DrinkPotion, "Ability.Action.UseItem.DrinkPotion");

	AddTag(Ability_Attack_MeleeCombo_A1, "Ability.Attack.MeleeCombo.A1");
	AddTag(Ability_Attack_MeleeCombo_A2, "Ability.Attack.MeleeCombo.A2");
	AddTag(Ability_Attack_MeleeCombo_A3, "Ability.Attack.MeleeCombo.A3");
	AddTag(Ability_Attack_MeleeCombo_A4, "Ability.Attack.MeleeCombo.A4");

	AddTag(Ability_Attack_MeleeCombo_B1, "Ability.Attack.MeleeCombo.B1");
	AddTag(Ability_Attack_MeleeCombo_B2, "Ability.Attack.MeleeCombo.B2");
	AddTag(Ability_Attack_MeleeCombo_B3, "Ability.Attack.MeleeCombo.B3");

	AddTag(Ability_Skill_Defense_Block, "Ability.Skill.Defense.Block");
	AddTag(Ability_Skill_Defense_Parry, "Ability.Skill.Defense.Parry");
	AddTag(Ability_Skill_Defense_Stagger, "Ability.Skill.Defense.Stagger");
	AddTag(Ability_Skill_LeapAttack, "Ability.Skill.LeapAttack");
	AddTag(Ability_Skill_ThrowAxe, "Ability.Skill.ThrowAxe");
	AddTag(Ability_Skill_Roar, "Ability.Skill.Roar");

	AddTag(Ability_HitReact_Light, "Ability.HitReact.Light");
	AddTag(Ability_HitReact_Heavy, "Ability.HitReact.Heavy");

	AddTag(GameplayCue_WeaponWave_Axe, "GameplayCue.WeaponWave.Axe");
	AddTag(GameplayCue_WeaponHit_Axe, "GameplayCue.WeaponHit.Axe");
	AddTag(GameplayCue_WeaponHit_Shield, "GameplayCue.WeaponHit.Shield");
	AddTag(GameplayCue_ShieldParry, "GameplayCue.ShieldParry");
	AddTag(GameplayCue_GroundSmash, "GameplayCue.GroundSmash");

	AddTag(Cooldown_Skill_LeapAttack, "Cooldown.Skill.LeapAttack");
	AddTag(Cooldown_Skill_ThrowAxe, "Cooldown.Skill.ThrowAxe");
	AddTag(Cooldown_Skill_Roar, "Cooldown.Skill.Roar");

	//
	AddTag(Attributes_Vital_Health, "Attributes.Vital.Health", "", true);
	AddTag(Attributes_Vital_Stamina, "Attributes.Vital.Stamina", "", true);
	AddTag(Attributes_Primary_Strength, "Attributes.Primary.Strength", "", true);
	AddTag(Attributes_Primary_Dexterity, "Attributes.Primary.Dexterity", "", true);
	AddTag(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "", true);
	AddTag(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "", true);
	AddTag(Attributes_Secondary_MaxStamina, "Attributes.Secondary.MaxStamina", "", true);
	AddTag(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "", true);
	AddTag(Attributes_Secondary_Evasive, "Attributes.Secondary.Evasive", "", true);
	AddTag(Attributes_Secondary_PhysicalResistance, "Attributes.Secondary.PhysicalResistance", "", true);
	AddTag(Attributes_Secondary_MagicResistance, "Attributes.Secondary.MagicResistance", "", true);
	AddTag(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "", true);
	AddTag(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "", true);
	AddTag(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "", true);
	AddTag(Attributes_Secondary_StaminaRegeneration, "Attributes.Secondary.StaminaRegeneration", "", true);
	AddTag(Attributes_Secondary_MovementSpeed, "Attributes.Secondary.MovementSpeed", "", true);
	AddTag(Attributes_Secondary_BaseDamage, "Attributes.Secondary.BaseDamage", "", true);

	AddTag(Damage_Physical, "Damage.Physical");
	AddTag(Damage_Magic, "Damage.Magic");

	DamageTypesToResistances.Add(Damage_Physical, Attributes_Secondary_PhysicalResistance);
	DamageTypesToResistances.Add(Damage_Magic, Attributes_Secondary_MagicResistance);

	AddTag(State_HitReact, "State.HitReact");
	AddTag(State_Blocking, "State.Blocking");
	AddTag(State_PrepareParry, "State.PrepareParry");

	//
	AddTag(Effect_Asset_Damage, "Effect.Asset.Damage");
	AddTag(Effect_Magnitude_1, "Effect.Magnitude.1");
	AddTag(Effect_Magnitude_Damage, "Effect.Magnitude.Damage");
	AddTag(Effect_Magnitude_Stamina, "Effect.Magnitude.Stamina");
	AddTag(Effect_Magnitude_Armor, "Effect.Magnitude.Armor");
	AddTag(Effect_Magnitude_MaxHealth, "Effect.Magnitude.MaxHealth");
	AddTag(Effect_Magnitude_IncomingXp, "Effect.Magnitude.IncomingXp");
	AddTag(Effect_Magnitude_IncomingGoldCoinCount, "Effect.Magnitude.IncomingGoldCoinCount");

	/**
	 * Item
	 */
	AddTag(Inventory_Entry_Bag, "Inventory.Entry.Bag");
	
	AddTag(Inventory_Entry_UseBar, "Inventory.Entry.UseBar");

	AddTag(Inventory_Entry_Equipment, "Inventory.Entry.Equipment");
	AddTag(Inventory_Entry_Equipment_Armor_Helmet, "Inventory.Entry.Equipment.Armor.Helmet");
	AddTag(Inventory_Entry_Equipment_Armor_Chestplate, "Inventory.Entry.Equipment.Armor.Chestplate");
	AddTag(Inventory_Entry_Equipment_Armor_Leggings, "Inventory.Entry.Equipment.Armor.Leggings");
	AddTag(Inventory_Entry_Equipment_Armor_Boots, "Inventory.Entry.Equipment.Armor.Boots");
	AddTag(Inventory_Entry_Equipment_Weapon, "Inventory.Entry.Equipment.Weapon");

	AddTag(ItemType_None, "ItemType.None");
	AddTag(ItemType_Food, "ItemType.Food");
	AddTag(ItemType_Equipment_Armor_Helmet, "ItemType.Equipment.Armor.Helmet");
	AddTag(ItemType_Equipment_Armor_Chestplate, "ItemType.Equipment.Armor.Chestplate");
	AddTag(ItemType_Equipment_Armor_Leggings, "ItemType.Equipment.Armor.Leggings");
	AddTag(ItemType_Equipment_Armor_Boots, "ItemType.Equipment.Armor.Boots");
	AddTag(ItemType_Equipment_Weapon, "ItemType.Equipment.Weapon");
	AddTag(ItemType_Equipment_Weapon_AxeAndShield, "ItemType.Equipment.Weapon.AxeAndShield");
	AddTag(ItemType_Equipment_Weapon_GreatAxe, "ItemType.Equipment.Weapon.GreatAxe");
	AddTag(ItemType_Equipment_Weapon_DualAxe, "ItemType.Equipment.Weapon.DualAxe");

	TagWeaponTypeMap.Add(ItemType_Equipment_Weapon_AxeAndShield, EAxePlayerWeaponType::AxeAndShield);
	TagWeaponTypeMap.Add(ItemType_Equipment_Weapon_GreatAxe, EAxePlayerWeaponType::GreatAxe);
	TagWeaponTypeMap.Add(ItemType_Equipment_Weapon_DualAxe, EAxePlayerWeaponType::DualAxe);

	/**
	 * Interact
	 */
	AddTag(Ability_Interaction_Activate, "Ability.Interaction.Activate");

	//
	OnTagsAdded();
}

void FAxeGameplayTags::OnTagsAdded()
{
	for (FGameplayTag& GameplayTag : AllAttributesList)
	{
		if (GameplayTag.ToString().Contains("Primary"))
		{
			AllPrimaryTagList.Add(GameplayTag);
		}
		if (GameplayTag.ToString().Contains("Secondary"))
		{
			AllSecondaryTagList.Add(GameplayTag);
		}
	}
}


void FAxeGameplayTags::InitNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	Instance.AddAllTags(GameplayTagsManager);
}


void FAxeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment,
                              bool bIsAttributesTag)
{
	if (TagComment == nullptr)
	{
		TagComment = TagName;
	}

	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TagName),
		FString(TEXT("(Native) ")) + FString(TagComment)
	);


	if (bIsAttributesTag)
	{
		AllAttributesList.Add(OutTag);
	}
}

EAxePlayerWeaponType FAxeGameplayTags::GetWeaponTypeByTag(const FGameplayTag& Tag) const
{
	const EAxePlayerWeaponType* AxePlayerWeaponType = TagWeaponTypeMap.Find(Tag);
	if (AxePlayerWeaponType)
	{
		return *AxePlayerWeaponType;
	}
	return EAxePlayerWeaponType::None;
}
