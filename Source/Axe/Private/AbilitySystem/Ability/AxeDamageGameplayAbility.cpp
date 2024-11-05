// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeDamageGameplayAbility.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"

void UAxeDamageGameplayAbility::ApplyDamage(AAxeCharacterBase* TargetCharacter)
{
	if (!IsValid(TargetCharacter))
	{
		return;
	}
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();

	if (!IsValid(AxeASC) || !IsValid(TargetASC))
	{
		return;
	}

	FDamageEffectParams DamageEffectParam;
	DamageEffectParam.BaseDamage = BaseDamage;
	DamageEffectParam.DamageEffectClass = DamageEffectClass;
	DamageEffectParam.DamageType = DamageType;
	DamageEffectParam.EffectLevel = EffectLevel;

	AxeASC->ApplyDamageEffect(TargetASC, DamageEffectParam);
}
