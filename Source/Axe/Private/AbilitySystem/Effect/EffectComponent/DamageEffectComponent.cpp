// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/EffectComponent/DamageEffectComponent.h"

#include "GameplayEffect.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "GameplayTag/AxeGameplayTags.h"

UDamageEffectComponent::UDamageEffectComponent()
{
}

void UDamageEffectComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                            FGameplayEffectSpec& GESpec,
                                                            FPredictionKey& PredictionKey) const
{
}
