// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/AxeGameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"

bool UAxeGameplayEffect::CanApplyAttributeModifiers(const UGameplayEffect* GameplayEffect, float Level,
                                                    const FGameplayEffectContextHandle& EffectContext,
                                                    const AAxeCharacterBase* ToTarget,
                                                    TArray<FGameplayAttribute>& NotApplyAttributes)
{
	FGameplayEffectSpec Spec(GameplayEffect, EffectContext, Level);

	Spec.CalculateModifierMagnitudes();

	UAttributeSet* AttributeSet = ToTarget->GetAttributeSet();

	for (int32 ModIdx = 0; ModIdx < Spec.Modifiers.Num(); ++ModIdx)
	{
		const FGameplayModifierInfo& ModDef = Spec.Def->Modifiers[ModIdx];
		const FModifierSpec& ModSpec = Spec.Modifiers[ModIdx];

		// It only makes sense to check additive operators
		if (ModDef.ModifierOp == EGameplayModOp::Additive)
		{
			if (!ModDef.Attribute.IsValid())
			{
				continue;
			}
			float CurrentValue = ModDef.Attribute.GetNumericValueChecked(AttributeSet);
			float CostValue = ModSpec.GetEvaluatedMagnitude();

			if (CurrentValue + CostValue < 0.f)
			{
				NotApplyAttributes.AddUnique(ModDef.Attribute);
			}
		}
	}
	if (NotApplyAttributes.Num() > 0)
	{
		return false;
	}
	return true;
}
