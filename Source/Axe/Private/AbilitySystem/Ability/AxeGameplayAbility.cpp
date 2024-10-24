// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AxeGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"

#define ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(FunctionName, ReturnValue)																				\
{																																						\
	if (!ensure(IsInstantiated()))																														\
	{																																					\
		ABILITY_LOG(Error, TEXT("%s: " #FunctionName " cannot be called on a non-instanced ability. Check the instancing policy."), *GetPathName());	\
		return ReturnValue;																																\
	}																																					\
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

void UAxeGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAxeGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

//
TObjectPtr<UAxeAbilitySystemComponent> UAxeGameplayAbility::GetAxeAbilitySystemComponentFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		return Cast<UAxeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
	}
	return nullptr;
}
