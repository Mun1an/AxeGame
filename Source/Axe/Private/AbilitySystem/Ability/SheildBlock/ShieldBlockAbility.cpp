// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/SheildBlock/ShieldBlockAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
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
	SetIsBlocking(true);
}

void UShieldBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//
	SetIsBlocking(false);
}

void UShieldBlockAbility::OnInputReleased(float TimeHeld)
{
	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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
