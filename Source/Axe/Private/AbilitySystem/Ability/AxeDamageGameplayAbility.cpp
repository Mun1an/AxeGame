// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeDamageGameplayAbility.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "Character/AxeCharacterBase.h"


void UAxeDamageGameplayAbility::ApplyDamageToTarget(AAxeCharacterBase* TargetCharacter, const FHitResult& HitResult)
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
	if (!DamageEffectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageEffectClass is not set in %s. Failed to apply damage."), *GetName());
		return;
	}
	FDamageEffectParams DamageEffectParam;

	DamageEffectParam.DamageEffectClass = DamageEffectClass;
	DamageEffectParam.DamageType = DamageType;
	DamageEffectParam.EffectLevel = EffectLevel;
	DamageEffectParam.HitResult = HitResult;

	DamageEffectParam.DamageValue = GetTotalDamage();

	AxeASC->ApplyDamageEffectToTarget(TargetASC, DamageEffectParam);
}

float UAxeDamageGameplayAbility::GetOwnerCharacterBaseDamageAttr()
{
	if (const AAxeCharacterBase* AxeCharacter = GetAxeCharacterOwner())
	{
		UAttributeSet* AttributeSet = AxeCharacter->GetAttributeSet();
		if (UAxeAttributeSet* AxeAttributeSet = Cast<UAxeAttributeSet>(AttributeSet))
		{
			return AxeAttributeSet->GetBaseDamage();
		}
	}
	return 0.f;
}

float UAxeDamageGameplayAbility::GetTotalDamage()
{
	const float BaseDamage = GetOwnerCharacterBaseDamageAttr();
	return FixedDamage + BaseDamageCoefficient * BaseDamage;
}
