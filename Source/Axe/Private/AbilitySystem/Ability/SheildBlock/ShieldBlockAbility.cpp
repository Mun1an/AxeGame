// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SheildBlock/ShieldBlockAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectApplied_Self.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"


void UShieldBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// AbilityTask_WaitInputRelease
	UAbilityTask_WaitInputRelease* AbilityTask_WaitInputRelease =
		UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	AbilityTask_WaitInputRelease->OnRelease.AddDynamic(this, &UShieldBlockAbility::OnInputReleased);
	AbilityTask_WaitInputRelease->ReadyForActivation();

	//
	UAbilityTask_WaitGameplayEffectApplied_Self* AbilityTask_WaitGameplayEffectApplied_Self =
		UAbilityTask_WaitGameplayEffectApplied_Self::WaitGameplayEffectAppliedToSelf(
			this,
			FGameplayTargetDataFilterHandle(),
			FGameplayTagRequirements(),
			FGameplayTagRequirements(),
			false,
			nullptr,
			false
		);
	AbilityTask_WaitGameplayEffectApplied_Self->OnApplied.AddDynamic(this, &UShieldBlockAbility::OnEffectApplied);
	AbilityTask_WaitGameplayEffectApplied_Self->ReadyForActivation();
	//
	SetIsBlocking(true);
	SetIsPrepareParry(true);

	UAbilityTask_WaitDelay* AbilityTask_WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, PrePareParryTime);
	AbilityTask_WaitDelay->OnFinish.AddDynamic(this, &UShieldBlockAbility::OnPrePareParryFinished);
	AbilityTask_WaitDelay->ReadyForActivation();
}

void UShieldBlockAbility::OnPrePareParryFinished()
{
	SetIsPrepareParry(false);
}

void UShieldBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//
	SetIsBlocking(false);
	SetIsPrepareParry(false);
}


void UShieldBlockAbility::OnInputReleased(float TimeHeld)
{
	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UShieldBlockAbility::OnEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
                                          FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayEffectSpec* GameplayEffectSpec = SpecHandle.Data.Get();

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	const UAttributeSet* AttributeSet = AxeASC->GetAttributeSet(UAxeAttributeSet::StaticClass());
	const UAxeAttributeSet* AxeAttributeSet = Cast<UAxeAttributeSet>(AttributeSet);

	TArray<FGameplayEffectModifiedAttribute> ModifiedAttributes = GameplayEffectSpec->ModifiedAttributes;
	for (FGameplayEffectModifiedAttribute& ModifiedAttribute : ModifiedAttributes)
	{
		if (ModifiedAttribute.Attribute == AxeAttributeSet->GetIncomingDamageAttribute())
		{
			OnIncomingDamageEffectApplied(Source, SpecHandle, ActiveHandle);
			break;
		}
	}
}

void UShieldBlockAbility::OnIncomingDamageEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
                                                        FActiveGameplayEffectHandle ActiveHandle)
{
	if (bIsPrepareParry)
	{
		TransformToShieldParry(Source);
	}
}

void UShieldBlockAbility::TransformToShieldParry(AActor* Source)
{
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	// self
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	if (ShieldParryAbilityClass)
	{
		AxeASC->TryActivateAbilityByClass(ShieldParryAbilityClass);
	}

	// Source
	AAxeCharacterBase* SourceAxeCharacterBase = Cast<AAxeCharacterBase>(Source);
	if (SourceAxeCharacterBase)
	{
		UAbilitySystemComponent* SourceASC = SourceAxeCharacterBase->GetAbilitySystemComponent();
		UAxeAbilitySystemComponent* SourceAxeASC = Cast<UAxeAbilitySystemComponent>(SourceASC);
		SourceAxeASC->TryActivateHitReactAbility(AxeGameplayTags.Ability_HitReact_Light, FHitResult());
	}
}

void UShieldBlockAbility::SetIsBlocking(bool NewIsBlocking)
{
	bIsBlocking = NewIsBlocking;

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterOwner->AbilitySystemComponent);
	if (NewIsBlocking)
	{
		AxeASC->AddLooseGameplayTag(AxeGameplayTags.State_Blocking);
	}
	else
	{
		AxeASC->RemoveLooseGameplayTag(AxeGameplayTags.State_Blocking);
	}
}

void UShieldBlockAbility::SetIsPrepareParry(bool NewIsPrepareParry)
{
	bIsPrepareParry = NewIsPrepareParry;

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AxeCharacterOwner->AbilitySystemComponent);
	if (NewIsPrepareParry)
	{
		AxeASC->AddLooseGameplayTag(AxeGameplayTags.State_PrepareParry);
	}
	else
	{
		AxeASC->RemoveLooseGameplayTag(AxeGameplayTags.State_PrepareParry);
	}
}
