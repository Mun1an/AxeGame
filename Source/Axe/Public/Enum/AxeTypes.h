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
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackVector = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	// UPROPERTY(BlueprintReadWrite)
	// float DebuffChance = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffDamage = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffDuration = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float DebuffFrequency = 0.f;
	// UPROPERTY(BlueprintReadWrite)
	// float KnockbackChance = 0.f;
};

UENUM(BlueprintType)
enum class EDamageSpecialExpression : uint8
{
	None,
	CriticalHit,
	BeEvaded,
	BeBlocked,
	BeParried,
};

USTRUCT(BlueprintType)
struct FAxeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	// bool IsCriticalHit() const { return bIsCriticalHit; }
	// void SetCriticalHit(const bool Value) { bIsCriticalHit = Value; }
	//
	// bool IsEvasive() const { return bIsEvasive; }
	// void SetEvasive(const bool Value) { bIsEvasive = Value; }
	//
	// bool IsBlocked() const { return bIsBlocked; }
	// void SetBlocked(const bool Value) { bIsBlocked = Value; }

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

	EDamageSpecialExpression GetDamageSpecialExpression() const { return DamageSpecialExpression; }

	void SetDamageSpecialExpression(EDamageSpecialExpression NewValue)
	{
		DamageSpecialExpression = NewValue;
	}

	FVector GetKnockbackVector() const { return KnockbackVector; }
	void SetKnockbackVector(const FVector& Value) { KnockbackVector = Value; }

	float GetKnockbackForceMagnitude() const { return KnockbackForceMagnitude; }
	void SetKnockbackForceMagnitude(const float NewValue) { KnockbackForceMagnitude = NewValue; }

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAxeGameplayEffectContext::StaticStruct();
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsEvasive = false;
	UPROPERTY()
	bool bIsBlocked = false;
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
	EDamageSpecialExpression DamageSpecialExpression = EDamageSpecialExpression::None;

	UPROPERTY()
	FVector KnockbackVector = FVector::ZeroVector;
	UPROPERTY()
	float KnockbackForceMagnitude = 1.f;
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
