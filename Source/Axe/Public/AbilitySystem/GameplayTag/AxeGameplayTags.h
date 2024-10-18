// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
static TArray<FName> NativeGameplayTagNames = {
	//InputTag
	"InputTag.LMB",
	"InputTag.RMB",
	"InputTag.1",
	"InputTag.2",
	"InputTag.3",
	"InputTag.4",
	//Ability
	"Ability.MeleeAttack",
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
