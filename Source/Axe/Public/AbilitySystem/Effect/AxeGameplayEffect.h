// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AxeGameplayEffect.generated.h"

class AAxeCharacterBase;
struct FGameplayAttribute;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AXE_API UAxeGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	bool CanApplyAttributeModifiers(const UGameplayEffect* GameplayEffect, float Level,
	                                const FGameplayEffectContextHandle& EffectContext,
	                                const AAxeCharacterBase* ToTarget,
	                                TArray<FGameplayAttribute>& NotApplyAttributes);
};
