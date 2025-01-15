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
	void ApplyDamageToTarget(AAxeCharacterBase* TargetCharacter, const FHitResult& HitResult = FHitResult());

protected:
	UFUNCTION(BlueprintCallable)
	float GetOwnerCharacterBaseDamageAttr();

	UFUNCTION(BlueprintCallable)
	float GetTotalDamage();

	UFUNCTION(BlueprintCallable)
	FVector GetDefaultKnockbackVector(AActor* TargetActor) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	FGameplayTag DamageType;

	// 固定伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float FixedDamage = 0.f;

	// 基础伤害系数，伤害 = 基础伤害系数 * BaseDamage属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float BaseDamageCoefficient = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float DamageToToughnessValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float EffectLevel = 1.f;

	UPROPERTY(BlueprintReadWrite, Category="Damage")
	FVector KnockbackVector = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float KnockbackForceMagnitude = 1.f;
};
