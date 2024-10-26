// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"

#include "GameplayTagsManager.h"

FAxeGameplayTags FAxeGameplayTags::Instance;

void FAxeGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_LightAttack);
	AddTag(InputTag_HeavyAttack);
	AddTag(InputTag_Skill_1);
	AddTag(InputTag_Skill_2);
	AddTag(InputTag_Skill_3);
	AddTag(InputTag_Skill_4);

	AddTag(Ability_LightAttack);
	AddTag(Ability_HeavyAttack);

	AddTag(Ability_MeleeCombo_A1);
	AddTag(Ability_MeleeCombo_A2);
	AddTag(Ability_MeleeCombo_A3);
	AddTag(Ability_MeleeCombo_A4);

	AddTag(Ability_MeleeCombo_B1);
	AddTag(Ability_MeleeCombo_B2);
	AddTag(Ability_MeleeCombo_B3);

	AddTag(Ability_Skill_LeapAttack);
}

void FAxeGameplayTags::InitNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	Instance.AddAllTags(GameplayTagsManager);
}

void FAxeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	if (TagName == nullptr)
	{
		FString NewTagName = OutTag.ToString().Replace(TEXT("_"), TEXT("."));
		TagName = TCHAR_TO_ANSI(*NewTagName);
	}
	if (TagComment == nullptr)
	{
		TagComment = TagName;
	}

	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TagName),
		FString(TEXT("(Native) ")) + FString(TagComment)
	);
}
