// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"

#include "GameplayEffectTypes.h"
#include "Enum/AxeTypes.h"

FAxeGameplayEffectContext& UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContextHandle& EffectContextHandle)
{
	FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
	FAxeGameplayEffectContext* AuraEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return *AuraEffectContext;
}

FAxeGameplayEffectContext* UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContext* EffectContext)
{
	FAxeGameplayEffectContext* AuraEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return AuraEffectContext;
}

const FAxeGameplayEffectContext* UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	const FGameplayEffectContext* EffectContext)
{
	const FAxeGameplayEffectContext* AxeGameplayEffectContext = static_cast<const FAxeGameplayEffectContext*>(EffectContext);
	return AxeGameplayEffectContext;
}
