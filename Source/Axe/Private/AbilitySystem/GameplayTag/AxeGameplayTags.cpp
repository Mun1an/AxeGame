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

	AddTag(Ability_LightAttack, "Ability.LightAttack");
	AddTag(Ability_HeavyAttack, "Ability.HeavyAttack");

	AddTag(Ability_MeleeCombo_A1, "Ability.MeleeCombo.A1");
	AddTag(Ability_MeleeCombo_A2, "Ability.MeleeCombo.A2");
	AddTag(Ability_MeleeCombo_A3, "Ability.MeleeCombo.A3");
	AddTag(Ability_MeleeCombo_A4, "Ability.MeleeCombo.A4");

	AddTag(Ability_MeleeCombo_B1, "Ability.MeleeCombo.B1");
	AddTag(Ability_MeleeCombo_B2, "Ability.MeleeCombo.B2");
	AddTag(Ability_MeleeCombo_B3, "Ability.MeleeCombo.B3");

	AddTag(Ability_Skill_LeapAttack, "Ability.Skill.LeapAttack");

	AddTag(GameplayCue_WeaponTrail, "GameplayCue.WeaponTrail");
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
