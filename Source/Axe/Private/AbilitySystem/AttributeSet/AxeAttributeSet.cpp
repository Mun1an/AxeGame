// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "Enum/AxeTypes.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/AxePlayerState.h"

UAxeAttributeSet::UAxeAttributeSet()
{
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Vital_Stamina, GetStaminaAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_Evasive, GetEvasiveAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_PhysicalResistance,
	                               GetPhysicalResistanceAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_MagicResistance, GetMagicResistanceAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_CriticalHitChance,
	                               GetCriticalHitChanceAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_CriticalHitDamage,
	                               GetCriticalHitDamageAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_HealthRegeneration,
	                               GetHealthRegenerationAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_StaminaRegeneration,
	                               GetStaminaRegenerationAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_MovementSpeed, GetMovementSpeedAttribute);
	TagsToAttributesFuncPtrMap.Add(AxeGameplayTags.Attributes_Secondary_BaseDamage, GetBaseDamageAttribute);
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
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Stamina, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, Evasive, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAxeAttributeSet, BaseDamage, COND_None, REPNOTIFY_Always);
}

void UAxeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
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
		}
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		const float AddedMaxHealth = NewValue - OldValue;
		const float NewHealth = FMath::Max(GetHealth() + AddedMaxHealth, 1.0f);
		SetHealth(NewHealth);
	}
	if (Attribute == GetMaxStaminaAttribute())
	{
		const float AddedMaxStamina = NewValue - OldValue;
		const float NewStamina = FMath::Max(GetStamina() + AddedMaxStamina, 1.0f);
		SetStamina(NewStamina);
	}
}

void UAxeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamageEffect(EffectProperties);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
	{
		HandleIncomingXpEffect(EffectProperties);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingGoldCoinCountAttribute())
	{
		HandleIncomingGoldCoinCountEffect(EffectProperties);
	}
}

/*
 * OnRep Functions
 */
void UAxeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Health, OldValue);
}

void UAxeAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, Stamina, OldValue);
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

void UAxeAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, MaxStamina, OldValue);
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

void UAxeAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, StaminaRegeneration, OldValue);
}

void UAxeAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, MovementSpeed, OldValue);
}

void UAxeAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAxeAttributeSet, BaseDamage, OldValue);
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
	const FGameplayEffectContext* GameplayEffectContext = Props.EffectContextHandle.Get();
	const FAxeGameplayEffectContext* AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		GameplayEffectContext);
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	const FHitResult* HitResult = AxeEffectContext->GetHitResult();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	const FVector KnockbackVector = AxeEffectContext->GetKnockbackVector();
	const float KnockbackForceMagnitude = AxeEffectContext->GetKnockbackForceMagnitude();

	//
	SetIncomingDamage(0.f);
	bool bFatal = false;
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		bFatal = NewHealth <= 0.f;
	}
	// ShowDamageFloatingText
	if (Props.SourceCharacter && Props.TargetCharacter)
	{
		ShowDamageFloatingText(
			Cast<AAxeCharacterBase>(Props.SourceCharacter),
			Cast<AAxeCharacterBase>(Props.TargetCharacter),
			LocalIncomingDamage,
			AxeEffectContext
		);
	}
	if (bFatal)
	{
		FVector DeathImpulse = KnockbackVector;
		DeathImpulse.Normalize();
		DeathImpulse.Z += 0.1;
		DeathImpulse *= KnockbackForceMagnitude * 20000;
		AxeCharacterOwner->SetDeathImpulseVector(DeathImpulse);
		AxeCharacterOwner->SetDeath();
		return;
	}
	// 死后return

	// HitReact
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(Props.TargetASC);
	if (LocalIncomingDamage > 0.f && AxeASC)
	{
		AxeASC->TryActivateHitReactAbility(AxeGameplayTags.Ability_HitReact_Light, *HitResult, Props.SourceCharacter);
	}
	// KnockBack
	if (LocalIncomingDamage > 0.f)
	{
		FVector LaunchVector = KnockbackVector * KnockbackForceMagnitude * 500;
		AxeCharacterOwner->LaunchCharacter(LaunchVector, false, false);
	}
}

void UAxeAttributeSet::HandleIncomingXpEffect(const FEffectProperties& Props)
{
	const float LocalIncomingXp = GetIncomingXp();

	const FGameplayEffectContext* GameplayEffectContext = Props.EffectContextHandle.Get();
	const FAxeGameplayEffectContext* AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		GameplayEffectContext);
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	//
	SetIncomingXp(0.f);

	//
	if (Props.TargetCharacter)
	{
		if (APlayerState* PlayerState = Props.TargetCharacter->GetPlayerState())
		{
			AAxePlayerState* AxePS = Cast<AAxePlayerState>(PlayerState);
			AxePS->AddToPlayerXp(LocalIncomingXp);
		}
	}
}

void UAxeAttributeSet::HandleIncomingGoldCoinCountEffect(const FEffectProperties& Props)
{
	const float LocalIncomingGoldCoinCount = GetIncomingGoldCoinCount();

	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	//
	SetIncomingGoldCoinCount(0.f);

	//
	if (Props.TargetCharacter)
	{
		if (APlayerState* PlayerState = Props.TargetCharacter->GetPlayerState())
		{
			AAxePlayerState* AxePS = Cast<AAxePlayerState>(PlayerState);
			AxePS->AddToGoldCoinCount(LocalIncomingGoldCoinCount);
		}
	}
}

//
void UAxeAttributeSet::ShowDamageFloatingText(AAxeCharacterBase* SourceCharacter, AAxeCharacterBase* TargetCharacter,
                                              const float Damage,
                                              const FAxeGameplayEffectContext* AxeEffectContext) const
{
	if (SourceCharacter == TargetCharacter)
	{
		return;
	}

	const FHitResult* HitResult = AxeEffectContext->GetHitResult();
	const FVector ShowLocation = HitResult->ImpactPoint;
	EDamageSpecialExpression DamageSpecialExpression = AxeEffectContext->GetDamageSpecialExpression();

	if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(SourceCharacter))
	{
		UActionCombatComponent* ActionCombatComponent = AxeCharacterPlayer->GetActionCombatComponent_Implementation();
		ActionCombatComponent->ShowDamageNumber(
			Damage, TargetCharacter, DamageSpecialExpression, ShowLocation
		);
	}
	if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(TargetCharacter))
	{
		UActionCombatComponent* ActionCombatComponent = AxeCharacterPlayer->GetActionCombatComponent_Implementation();
		ActionCombatComponent->ShowDamageNumber(
			Damage, TargetCharacter, DamageSpecialExpression, ShowLocation
		);
	}
}
