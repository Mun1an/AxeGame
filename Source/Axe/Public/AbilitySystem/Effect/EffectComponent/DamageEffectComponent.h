// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectComponent.h"
#include "Enum/AxeTypes.h"
#include "DamageEffectComponent.generated.h"


struct FDamageEffectParams;
struct FGameplayEffectRemovalInfo;
struct FActiveGameplayEffectHandle;
class UGameplayAbility;

UCLASS()
class AXE_API UDamageEffectComponent : public UGameplayEffectComponent
{
	friend class UGameplayEffect; // for upgrade path

	GENERATED_BODY()

public:
	/** Constructor */
	UDamageEffectComponent();

	virtual void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
	                                      FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
	UFUNCTION()
	const FDamageEffectParams& GetDamageEffectParams() const { return DamageEffectParams; }

protected:
	UPROPERTY(EditDefaultsOnly)
	FDamageEffectParams DamageEffectParams;
};
