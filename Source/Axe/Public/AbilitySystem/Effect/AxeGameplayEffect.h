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
	void OnBeforeEffectApplied(FGameplayEffectSpec& Spec) const;
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnBeforeEffectApplied(FGameplayEffectSpec& Spec) const;
};
