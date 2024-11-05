// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UAxeAttributeSet::UAxeAttributeSet()
{
}

AAxeCharacterBase* UAxeAttributeSet::GetAxeCharacterOwner() const
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponentChecked();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	return AxeASC->GetAxeCharacterOwner();
}

void UAxeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Health, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Evasive, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UAxeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp Health
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UAxeAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	//
	if (Attribute == GetMovementSpeedAttribute())
	{
		AAxeCharacterBase* AxeCharacterBase = GetAxeCharacterOwner();
		if (AxeCharacterBase)
		{
			AxeCharacterBase->SetWalkSpeed(NewValue);
			AxeCharacterBase->SetRotationRateByWalkSpeed();
		}
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		const float AddedMaxHealth = NewValue - OldValue;
		const float NewHealth = FMath::Max(GetHealth() + AddedMaxHealth, 1.0f);
		SetHealth(NewHealth);
	}
}

void UAxeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

/*
 * OnRep Functions
 */
void UAxeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Health, OldValue);
}

void UAxeAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Strength, OldValue);
}

void UAxeAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Dexterity, OldValue);
}

void UAxeAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Intelligence, OldValue);
}

//
void UAxeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, MaxHealth, OldValue);
}

void UAxeAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Armor, OldValue);
}

void UAxeAttributeSet::OnRep_Evasive(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Evasive, OldValue);
}

void UAxeAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, PhysicalResistance, OldValue);
}

void UAxeAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, MagicResistance, OldValue);
}

void UAxeAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, CriticalHitChance, OldValue);
}

void UAxeAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, CriticalHitDamage, OldValue);
}

void UAxeAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, HealthRegeneration, OldValue);
}

void UAxeAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, MovementSpeed, OldValue);
}
