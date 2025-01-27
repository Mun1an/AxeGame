// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/AxeGameplayEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "AbilitySystem/Effect/EffectComponent/DamageEffectComponent.h"
#include "GameplayTag/AxeGameplayTags.h"

void UAxeGameplayEffect::OnBeforeEffectApplied(FGameplayEffectSpec& Spec) const
{
	// CDO
	if (GEComponents.Num() > 0)
	{
		const UDamageEffectComponent* DamageEffectComponent = FindComponent<UDamageEffectComponent>();
		if (DamageEffectComponent)
		{
			const FDamageEffectParams& DamageEffectParams = DamageEffectComponent->GetDamageEffectParams();
			FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

			ContextHandle.AddHitResult(DamageEffectParams.HitResult);
			FAxeGameplayEffectContext& AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
				ContextHandle
			);
			AxeEffectContext.SetKnockbackVector(DamageEffectParams.KnockbackVector);
			AxeEffectContext.SetKnockbackForceMagnitude(DamageEffectParams.KnockbackForceMagnitude);
			AxeEffectContext.SetDamageToToughnessValue(DamageEffectParams.DamageToToughnessValue);

			Spec.SetSetByCallerMagnitude(DamageEffectParams.DamageType, DamageEffectParams.DamageValue);

			Spec.SetContext(ContextHandle);
		}
	}
}
