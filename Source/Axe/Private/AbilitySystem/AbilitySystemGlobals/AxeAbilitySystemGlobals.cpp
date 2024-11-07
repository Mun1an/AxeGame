// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemGlobals/AxeAbilitySystemGlobals.h"

#include "Enum/AxeTypes.h"

FGameplayEffectContext* UAxeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAxeGameplayEffectContext();
}
