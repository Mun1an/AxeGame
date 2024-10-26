// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FAxeGameplayTags
{
	FGameplayTag InputTag_LightAttack;
	FGameplayTag InputTag_HeavyAttack;
	FGameplayTag InputTag_Skill_1;
	FGameplayTag InputTag_Skill_2;
	FGameplayTag InputTag_Skill_3;
	FGameplayTag InputTag_Skill_4;

	FGameplayTag Ability_LightAttack;
	FGameplayTag Ability_HeavyAttack;

	FGameplayTag Ability_MeleeCombo_A1;
	FGameplayTag Ability_MeleeCombo_A2;
	FGameplayTag Ability_MeleeCombo_A3;
	FGameplayTag Ability_MeleeCombo_A4;

	FGameplayTag Ability_MeleeCombo_B1;
	FGameplayTag Ability_MeleeCombo_B2;
	FGameplayTag Ability_MeleeCombo_B3;

	FGameplayTag Ability_Skill_LeapAttack;

	static void InitNativeGameplayTags();

	static FAxeGameplayTags& Get()
	{
		return Instance;
	}

protected:
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName = nullptr, const ANSICHAR* TagComment = nullptr);
	void AddAllTags(UGameplayTagsManager& Manager);

private:
	static FAxeGameplayTags Instance;
};
