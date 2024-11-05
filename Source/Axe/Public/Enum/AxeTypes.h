#pragma once
#include "GameplayTagContainer.h"
#include "AxeTypes.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float EffectLevel = 1.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffChance = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffDamage = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffDuration = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffFrequency = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float KnockbackForceMagnitude = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// FVector KnockbackForce = FVector::ZeroVector;
	// UPROPERTY(BlueprintReadWrite)
	// float KnockbackChance = 0.f;
};
