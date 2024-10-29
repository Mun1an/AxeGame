// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Anim/ComboAnimNotifyState.h"
#include "Anim/ComboInputCacheAnimNotifyState.h"
#include "Anim/HitTraceAnimNotifyState.h"
#include "Anim/LaunchCharacterNotifyState.h"
#include "Anim/MovementSlowAnimNotifyState.h"
#include "Character/AxeCharacterBase.h"
#include "Interaction/CombatInterface.h"

#define ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(FunctionName, ReturnValue)																				\
{																																						\
	if (!ensure(IsInstantiated()))																														\
	{																																					\
		ABILITY_LOG(Error, TEXT("%s: " #FunctionName " cannot be called on a non-instanced ability. Check the instancing policy."), *GetPathName());	\
		return ReturnValue;																																\
	}																																					\
}

AAxeCharacterBase* UAxeGameplayAbility::GetAxeCharacterOwner() const
{
	if (AActor* LocalAvatarActor = CurrentActorInfo->AvatarActor.Get())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = CurrentActorInfo->OwnerActor.Get();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

bool UAxeGameplayAbility::CanChangeActivationGroup(EAxeAbilityActivationGroup NewGroup) const
{
	if (!IsInstantiated() || !IsActive())
	{
		return false;
	}

	if (ActivationGroup == NewGroup)
	{
		return true;
	}

	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();
	check(AxeASC);

	if ((ActivationGroup != EAxeAbilityActivationGroup::Exclusive_Blocking) &&
		AxeASC->IsActivationGroupBlocked(NewGroup, this)
	)
	{
		return false;
	}

	if ((NewGroup == EAxeAbilityActivationGroup::Exclusive_Replaceable) && !CanBeCanceled())
	{
		return false;
	}
	if ((NewGroup == EAxeAbilityActivationGroup::Exclusive_ReplaceableByCondition) && !CanBeCanceled())
	{
		return false;
	}

	return true;
}

bool UAxeGameplayAbility::ChangeActivationGroup(EAxeAbilityActivationGroup NewGroup)
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(ChangeActivationGroup, false);

	if (!CanChangeActivationGroup(NewGroup))
	{
		return false;
	}

	if (ActivationGroup != NewGroup)
	{
		UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponentFromActorInfo();

		AxeASC->RemoveAbilityFromActivationGroup(ActivationGroup, this);
		AxeASC->AddAbilityToActivationGroup(NewGroup, this);
		ActivationGroup = NewGroup;
	}

	return true;
}

bool UAxeGameplayAbility::CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const
{
	return true;
}

bool UAxeGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags,
                                             FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (AxeASC->IsActivationGroupBlocked(ActivationGroup, this))
	{
		return false;
	}

	return true;
}

void UAxeGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UAxeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Tasks
	AddTask();
}

void UAxeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


//
UAxeAbilitySystemComponent* UAxeGameplayAbility::GetAxeAbilitySystemComponentFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		return Cast<UAxeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	}
	return nullptr;
}

UActionCombatComponent* UAxeGameplayAbility::GetActionCombatComponent() const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (CombatInterface)
		{
			return CombatInterface->GetActionCombatComponent();
		}
	}
	return nullptr;
}

UComboActionComponent* UAxeGameplayAbility::GetComboActionComponent() const
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	if (AxeCharacterOwner)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
		if (CombatInterface)
		{
			return CombatInterface->GetComboActionComponent();
		}
	}
	return nullptr;
}

void UAxeGameplayAbility::AddTask()
{
	//AddTask

	//UAbilityTask_MontageNotify

	// MovementSlow
	UAbilityTask_MontageNotify* AT_MovementSlow_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, AbilityMontage,
		UMovementSlowAnimNotifyState::StaticClass()
	);
	AT_MovementSlow_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MovementSlow_NotifyBegin);
	AT_MovementSlow_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_MovementSlow_NotifyEnd);
	AT_MovementSlow_Ans->ReadyForActivation();

	// LaunchCharacter
	UAbilityTask_MontageNotify* AT_LaunchCharacter_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, AbilityMontage,
		ULaunchCharacterNotifyState::StaticClass()
	);
	AT_LaunchCharacter_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin);
	AT_LaunchCharacter_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd);
	AT_LaunchCharacter_Ans->ReadyForActivation();

	// ComboInputCache
	UAbilityTask_MontageNotify* AT_ComboInputCache_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, AbilityMontage,
		UComboInputCacheAnimNotifyState::StaticClass()
	);
	AT_ComboInputCache_Ans->MontageNotifyStartDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_ComboInputCache_NotifyBegin);
	AT_ComboInputCache_Ans->MontageNotifyEndDelegate.AddDynamic(
		this, &UAxeGameplayAbility::Ans_ComboInputCache_NotifyEnd);
	AT_ComboInputCache_Ans->ReadyForActivation();

	// Combo
	UAbilityTask_MontageNotify* AT_Combo_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, AbilityMontage,
		UComboAnimNotifyState::StaticClass()
	);
	AT_Combo_Ans->MontageNotifyStartDelegate.AddDynamic(this, &UAxeGameplayAbility::Ans_Combo_NotifyBegin);
	AT_Combo_Ans->MontageNotifyEndDelegate.AddDynamic(this, &UAxeGameplayAbility::Ans_Combo_NotifyEnd);
	AT_Combo_Ans->ReadyForActivation();

	// HitTrace
	UAbilityTask_MontageNotify* AT_HitTrace_Ans = UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(
		this, AbilityMontage,
		UHitTraceAnimNotifyState::StaticClass()
	);
	AT_HitTrace_Ans->MontageNotifyStartDelegate.AddDynamic(this, &UAxeGameplayAbility::Ans_HitTrace_NotifyBegin);
	AT_HitTrace_Ans->MontageNotifyEndDelegate.AddDynamic(this, &UAxeGameplayAbility::Ans_HitTrace_NotifyEnd);
	AT_HitTrace_Ans->ReadyForActivation();
}

void UAxeGameplayAbility::Ans_MovementSlow_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	if (ActionCombatComponent)
	{
		ActionCombatComponent->ApplyMovementSlowEffectInAbilityUse(
			AbilityUsingMovementSlowEffectMagnitude,
			// fixme 不太优雅
			AbilityMontage->GetPlayLength()
		);
	}
}

void UAxeGameplayAbility::Ans_MovementSlow_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	if (ActionCombatComponent)
	{
		ActionCombatComponent->RemoveMovementSlowEffectInAbilityUse();
	}
}

void UAxeGameplayAbility::Ans_LaunchCharacter_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	UActionCombatComponent* ActionCombatComponent = GetActionCombatComponent();
	ULaunchCharacterNotifyState* LaunchCharacterNotifyState = Cast<ULaunchCharacterNotifyState>(AnimNotifyState);
	if (ActionCombatComponent && LaunchCharacterNotifyState)
	{
		ActionCombatComponent->SetCustomLaunchCharacter(
			LaunchCharacterNotifyState->LaunchSpeed,
			LaunchCharacterNotifyState->LaunchDirection,
			LaunchCharacterNotifyState->bXYOverride,
			LaunchCharacterNotifyState->bZOverride
		);
	}
}

void UAxeGameplayAbility::Ans_LaunchCharacter_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
}

void UAxeGameplayAbility::Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	UComboActionComponent* ComboActionComponent = GetComboActionComponent();
	if (ComboActionComponent)
	{
		ComboActionComponent->AnsComboInputCacheStart();
	}
}

void UAxeGameplayAbility::Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	UComboActionComponent* ComboActionComponent = GetComboActionComponent();
	if (ComboActionComponent)
	{
		ComboActionComponent->AnsComboInputCacheEnd();
	}
}

void UAxeGameplayAbility::Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	UComboActionComponent* ComboActionComponent = GetComboActionComponent();
	if (ComboActionComponent)
	{
		ComboActionComponent->AnsComboSwitchWindowStart(AnimNotifyState);
	}
}

void UAxeGameplayAbility::Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	UComboActionComponent* ComboActionComponent = GetComboActionComponent();
	if (ComboActionComponent)
	{
		ComboActionComponent->AnsComboSwitchWindowEnd(AnimNotifyState);
	}
}

void UAxeGameplayAbility::Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState)
{
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AxeCharacterOwner);
	if (!CombatInterface)
	{
		return;
	}

	UStaticMeshComponent* WeaponMeshComponent = CombatInterface->GetWeaponComponent();
	AbilityTask_HitTrace = UAbilityTask_HitTrace::CreateHitTraceTask(
		this, AxeCharacterOwner,
		WeaponMeshComponent,
		FName("Top"),
		FName("Bottom"),
		1.f,
		true
	);
	AbilityTask_HitTrace->ReadyForActivation();
}

void UAxeGameplayAbility::Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState)
{
	if (IsValid(AbilityTask_HitTrace))
	{
		AbilityTask_HitTrace->EndTask();
	}
}
