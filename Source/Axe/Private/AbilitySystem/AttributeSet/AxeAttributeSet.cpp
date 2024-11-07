// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "Enum/AxeTypes.h"
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
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
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
	if (Attribute == GetIncomingDamageAttribute())
	{
		// const float LocalIncomingDamage = GetIncomingDamage();
		// if (LocalIncomingDamage > 0)
		// {
		// 	SetIncomingDamage(0.f);
		// 	const float NewHealth = GetHealth() - LocalIncomingDamage;
		// 	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		// }
	}
}

void UAxeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamageEffect(EffectProperties);
	}
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

void UAxeAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (Props.SourceASC)
	{
		Props.SourceCharacter = Cast<UAxeAbilitySystemComponent>(Props.SourceASC)->GetAxeCharacterOwner();
	}
	if (Data.Target.AbilityActorInfo.IsValid())
	{
		AActor* AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AActor* OwnerActor = Data.Target.AbilityActorInfo->OwnerActor.Get();
		if (AvatarActor && Cast<AAxeCharacterBase>(AvatarActor))
		{
			Props.TargetCharacter = Cast<AAxeCharacterBase>(AvatarActor);
		}
		if (OwnerActor && Cast<AAxeCharacterBase>(OwnerActor))
		{
			Props.TargetCharacter = Cast<AAxeCharacterBase>(OwnerActor);
		}
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetCharacter);
	}
}

void UAxeAttributeSet::HandleIncomingDamageEffect(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		const bool bFatal = NewHealth <= 0.f;
	}
	const FGameplayEffectContext* GameplayEffectContext = Props.EffectContextHandle.Get();
	const FAxeGameplayEffectContext* AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		GameplayEffectContext);

	bool bIsCriticalHit = AxeEffectContext->IsCriticalHit();
	bool bIsEvasiveHit = AxeEffectContext->IsEvasive();

	const FHitResult* HitResult = Props.EffectContextHandle.Get()->GetHitResult();
	FVector ShowLocation = HitResult->ImpactPoint;

	if (Props.SourceCharacter && Props.TargetCharacter)
	{
		ShowDamageFloatingText(
			Cast<AAxeCharacterBase>(Props.SourceCharacter),
			Cast<AAxeCharacterBase>(Props.TargetCharacter),
			LocalIncomingDamage, bIsCriticalHit, bIsEvasiveHit, ShowLocation
		);
	}
}

//
void UAxeAttributeSet::ShowDamageFloatingText(AAxeCharacterBase* SourceCharacter, AAxeCharacterBase* TargetCharacter,
                                              const float Damage, const bool bIsCriticalHit, const bool bIsEvasiveHit,
                                              const FVector& ShowLocation) const
{
	if (SourceCharacter == TargetCharacter)
	{
		return;
	}
	AAxeCharacterPlayer* AxeCharacterPlayer = nullptr;
	if (Cast<AAxeCharacterPlayer>(SourceCharacter))
	{
		AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(SourceCharacter);
	}
	else if (Cast<AAxeCharacterPlayer>(TargetCharacter))
	{
		AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(TargetCharacter);
	}
	if (AxeCharacterPlayer)
	{
		UActionCombatComponent* ActionCombatComponent = AxeCharacterPlayer->GetActionCombatComponent();
		ActionCombatComponent->ShowDamageNumber(Damage, TargetCharacter, bIsCriticalHit, bIsEvasiveHit, ShowLocation);
	}
}
