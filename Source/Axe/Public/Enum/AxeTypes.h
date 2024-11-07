#pragma once
#include "GameplayEffectTypes.h"
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
	float DamageValue = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float EffectLevel = 1.f;
	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult = FHitResult();

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

USTRUCT(BlueprintType)
struct FAxeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetCriticalHit(const bool Value) { bIsCriticalHit = Value; }

	bool IsEvasive() const { return bIsEvasive; }
	void SetEvasive(const bool Value) { bIsEvasive = Value; }

	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	void SetSuccessfulDebuff(const bool Value) { bIsSuccessfulDebuff = Value; }

	float GetDebuffChance() const { return DebuffChance; }
	void SetDebuffChance(const float Value) { DebuffChance = Value; }

	float GetDebuffDamage() const { return DebuffDamage; }
	void SetDebuffDamage(const float Value) { DebuffDamage = Value; }

	float GetDebuffDuration() const { return DebuffDuration; }
	void SetDebuffDuration(const float Value) { DebuffDuration = Value; }

	float GetDebuffFrequency() const { return DebuffFrequency; }
	void SetDebuffFrequency(const float Value) { DebuffFrequency = Value; }

	FGameplayTag GetDamageType() const { return *DamageType.Get(); }
	void SetDamageType(const TSharedPtr<FGameplayTag>& Value) { DamageType = Value; }
	void SetDamageType(const FGameplayTag& Value) { DamageType = MakeShared<FGameplayTag>(Value); }

	FVector GetKnockbackForce() const { return KnockbackForce; }
	void SetKnockbackForce(const FVector& Value) { KnockbackForce = Value; }

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsEvasive = false;
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	UPROPERTY()
	float DebuffChance = 0.f;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType = nullptr;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};

template <>
struct TStructOpsTypeTraits<FAxeGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAxeGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
