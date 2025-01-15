// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AxeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

struct FAxeGameplayEffectContext;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};


class AAxeCharacterBase;
/**
 * 
 */
UCLASS()
class AXE_API UAxeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAxeAttributeSet();

	AAxeCharacterBase* GetAxeCharacterOwner() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	const TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>>& GetTagsToAttributesFuncPtrMap() const
	{
		return TagsToAttributesFuncPtrMap;
	}
	FGameplayTag GetAttributeTagByAttributeName(const FString& AttributeName) const;

	/**
	 * @brief Vital Attributes
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Health);
	UPROPERTY(ReplicatedUsing=OnRep_Stamina, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Stamina);
	UPROPERTY(ReplicatedUsing=OnRep_Toughness, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Toughness);
	/**
	 * @brief Primary Attributes
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Strength);

	UPROPERTY(ReplicatedUsing=OnRep_Dexterity, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Dexterity);

	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Intelligence);
	/**
	 * @brief Secondary Attributes
	 */
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, MaxHealth);
	UPROPERTY(ReplicatedUsing=OnRep_MaxStamina, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, MaxStamina);
	UPROPERTY(ReplicatedUsing=OnRep_MaxToughness, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxToughness;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, MaxToughness);

	UPROPERTY(ReplicatedUsing=OnRep_Armor, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Armor);

	UPROPERTY(ReplicatedUsing=OnRep_Evasive, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Evasive;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, Evasive);

	UPROPERTY(ReplicatedUsing=OnRep_PhysicalResistance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, PhysicalResistance);

	UPROPERTY(ReplicatedUsing=OnRep_MagicResistance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, MagicResistance);

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, CriticalHitChance);

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, CriticalHitDamage);

	UPROPERTY(ReplicatedUsing=OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, HealthRegeneration);

	UPROPERTY(ReplicatedUsing=OnRep_StaminaRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, StaminaRegeneration);

	UPROPERTY(ReplicatedUsing=OnRep_ToughnessRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ToughnessRegeneration;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, ToughnessRegeneration);

	UPROPERTY(ReplicatedUsing=OnRep_MovementSpeed, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, MovementSpeed);

	UPROPERTY(ReplicatedUsing=OnRep_BaseDamage, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, BaseDamage);
	/**
	 * @brief Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXp;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, IncomingXp);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingGoldCoinCount;
	ATTRIBUTE_ACCESSORS(UAxeAttributeSet, IncomingGoldCoinCount);

	/**
	 * @brief OnRep func
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldValue) const;

	// Primary
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue) const;

	// Secondary
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxToughness(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Evasive(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MagicResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ToughnessRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue) const;

private:
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributesFuncPtrMap;

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void HandleIncomingDamageEffect(const FEffectProperties& Props);
	void HandleIncomingXpEffect(const FEffectProperties& Props);
	void HandleIncomingGoldCoinCountEffect(const FEffectProperties& Props);

	void ShowDamageFloatingText(AAxeCharacterBase* SourceCharacter, AAxeCharacterBase* TargetCharacter,
	                            const float Damage,
	                            const FAxeGameplayEffectContext* AxeEffectContext) const;
};
