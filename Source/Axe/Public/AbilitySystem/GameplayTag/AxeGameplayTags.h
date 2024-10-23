// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
static TArray<FName> NativeGameplayTagNames = {
	//InputTag
	"InputTag.LightAttack",
	"InputTag.HeavyAttack",
	"InputTag.Skill.1",
	"InputTag.Skill.2",
	"InputTag.Skill.3",
	"InputTag.Skill.4",
	//Ability
	"Ability.LightAttack",
	"Ability.HeavyAttack",

	"Ability.MeleeCombo.A1",
	"Ability.MeleeCombo.A2",
	"Ability.MeleeCombo.A3",
	"Ability.MeleeCombo.A4",
	
	"Ability.MeleeCombo.B1",
	"Ability.MeleeCombo.B2",
	"Ability.MeleeCombo.B3",
};

struct FAxeGameplayTags
{
	static void InitNativeGameplayTags();

	static FAxeGameplayTags& Get()
	{
		return Instance;
	}

private:
	static FAxeGameplayTags Instance;
};
