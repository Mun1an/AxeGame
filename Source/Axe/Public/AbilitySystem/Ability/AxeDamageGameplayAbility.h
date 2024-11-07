// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "Enum/AxeTypes.h"
#include "AxeDamageGameplayAbility.generated.h"

struct FHitResult;
/**
 * 
 */
UCLASS()
class AXE_API UAxeDamageGameplayAbility : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(AAxeCharacterBase* TargetCharacter, const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float EffectLevel = 1.f;
};
