// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"

#include "GameplayTasksComponent.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "Anim/AN/AxeAnimNotifyBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/AxeCharacterBase.h"

UAbilityTask_MontageNotify* UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(UGameplayAbility* OwningAbility,
	UAnimMontage* AbilityMontage, TSubclassOf<UAnimNotifyState> NotifyStateClass)
{
	UAbilityTask_MontageNotify* MyObj = NewAbilityTask<UAbilityTask_MontageNotify>(OwningAbility);
	const UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(OwningAbility);
	MyObj->AxeCharacter = AxeGameplayAbility->GetAxeCharacterOwner();
	MyObj->AbilityMontage = AbilityMontage;
	MyObj->NotifyStateClass = NotifyStateClass;
	return MyObj;
}

UAbilityTask_MontageNotify* UAbilityTask_MontageNotify::CreateMontageNotifyTask(UGameplayAbility* OwningAbility,
	UAnimMontage* AbilityMontage, TSubclassOf<UAnimNotify> NotifyClass)
{
	UAbilityTask_MontageNotify* MyObj = NewAbilityTask<UAbilityTask_MontageNotify>(OwningAbility);
	const UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(OwningAbility);
	MyObj->AxeCharacter = AxeGameplayAbility->GetAxeCharacterOwner();
	MyObj->AbilityMontage = AbilityMontage;
	MyObj->NotifyClass = NotifyClass;
	return MyObj;
}

void UAbilityTask_MontageNotify::InitTaskAuthority()
{
	if (Ability)
	{
		const FGameplayAbilityActivationInfo ActivationInfo = Ability->GetCurrentActivationInfo();
		bHasAuthority = Ability->HasAuthority(&ActivationInfo);
	}
}


void UAbilityTask_MontageNotify::Activate()
{
	Super::Activate();

	if (!IsValid(AbilityMontage))
	{
		return;
	}
	if (!IsValid(NotifyClass) && !IsValid(NotifyStateClass))
	{
		return;
	}

	InitTaskAuthority();

	TArray<FAnimNotifyEvent> AnimNotifyEvents = AbilityMontage->Notifies;

	if (IsValid(NotifyClass))
	{
		NotifyActivate(AnimNotifyEvents);
	}
	if (IsValid(NotifyStateClass))
	{
		NotifyStateActivate(AnimNotifyEvents);
	}
}

void UAbilityTask_MontageNotify::OnDestroy(bool AbilityEnded)
{
	NotifyEnd();
	NotifyStateEnd();

	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_MontageNotify::NotifyActivate(TArray<FAnimNotifyEvent> AnimNotifyEvents)
{
	for (FAnimNotifyEvent& AnimNotifyEvent : AnimNotifyEvents)
	{
		if (AnimNotifyEvent.Notify.GetClass() == NotifyClass)
		{
			NotifyList.Add(AnimNotifyEvent.Notify);
		}
	}
	if (NotifyList.Num() == 0)
	{
		return;
	}
	for (UAnimNotify* AnimNotify : NotifyList)
	{
		UAxeAnimNotifyBase* AnimNotifyBase = Cast<UAxeAnimNotifyBase>(AnimNotify);
		if (!IsValid(AnimNotifyBase))
		{
			continue;
		}
		FDelegateHandle BeginDelegateHandle = AnimNotifyBase->AnimNotifyBeginDelegate.AddUObject(
			this, &UAbilityTask_MontageNotify::AnimNotifyBegin
		);
		TArray<FDelegateHandle>& DelegateHandleList = AN_DelegateHandleMap.FindOrAdd(AnimNotify);
		DelegateHandleList.AddUnique(BeginDelegateHandle);
	}
}

void UAbilityTask_MontageNotify::NotifyEnd()
{
	if (NotifyList.Num() > 0)
	{
		for (UAnimNotify* AnimNotify : NotifyList)
		{
			TArray<FDelegateHandle>* DelegateHandleList = AN_DelegateHandleMap.Find(AnimNotify);
			if (DelegateHandleList == nullptr)
			{
				continue;
			}

			UAxeAnimNotifyBase* AnimNotifyBase = Cast<UAxeAnimNotifyBase>(AnimNotify);
			if (!IsValid(AnimNotifyBase))
			{
				continue;
			}
			// remove delegate
			for (FDelegateHandle DelegateHandle : *DelegateHandleList)
			{
				AnimNotifyBase->AnimNotifyBeginDelegate.Remove(DelegateHandle);
			}
		}
	}
}

void UAbilityTask_MontageNotify::NotifyStateActivate(TArray<FAnimNotifyEvent> AnimNotifyEvents)
{
	for (FAnimNotifyEvent& AnimNotifyEvent : AnimNotifyEvents)
	{
		if (AnimNotifyEvent.NotifyStateClass.GetClass() == NotifyStateClass)
		{
			NotifyStateList.Add(AnimNotifyEvent.NotifyStateClass);
		}
	}
	if (NotifyStateList.Num() == 0)
	{
		return;
	}
	for (UAnimNotifyState* AnimNotifyState : NotifyStateList)
	{
		UAxeAnimNotifyStateBase* AnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(AnimNotifyState);
		if (!IsValid(AnimNotifyStateBase))
		{
			continue;
		}
		FDelegateHandle BeginDelegateHandle = AnimNotifyStateBase->AnimNotifyStateBeginDelegate.AddUObject(
			this, &UAbilityTask_MontageNotify::AnimNotifyStateBegin
		);
		FDelegateHandle EndDelegateHandle = AnimNotifyStateBase->AnimNotifyStateEndDelegate.AddUObject(
			this, &UAbilityTask_MontageNotify::AnimNotifyStateEnd
		);
		TArray<FDelegateHandle>& DelegateHandleList = ANS_DelegateHandleMap.FindOrAdd(AnimNotifyState);
		DelegateHandleList.AddUnique(BeginDelegateHandle);
		DelegateHandleList.AddUnique(EndDelegateHandle);
	}
}

void UAbilityTask_MontageNotify::NotifyStateEnd()
{
	if (NotifyStateList.Num() > 0)
	{
		for (UAnimNotifyState* AnimNotifyState : NotifyStateList)
		{
			TArray<FDelegateHandle>* DelegateHandleList = ANS_DelegateHandleMap.Find(AnimNotifyState);
			if (DelegateHandleList == nullptr)
			{
				continue;
			}

			UAxeAnimNotifyStateBase* AnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(AnimNotifyState);
			if (!IsValid(AnimNotifyStateBase))
			{
				continue;
			}

			// remove delegate
			for (FDelegateHandle DelegateHandle : *DelegateHandleList)
			{
				AnimNotifyStateBase->AnimNotifyStateBeginDelegate.Remove(DelegateHandle);
			}
			// 被打断
			// 如果在NotifyStateEnded的广播到达前，Task因为Ability的结束而被销毁, 那么NotifyStateEnded的广播不会到达，需要手动调用
			const bool bIsNotifyStateEnded = AnimNotifyStateBase->GetIsNotifyStateEnded();
			if (!bIsNotifyStateEnded)
			{
				AnimNotifyStateBase->SetIsInterrupted(true);
				AnimNotifyStateEnd(AnimNotifyStateBase);
			}
		}
	}
}


void UAbilityTask_MontageNotify::AnimNotifyBegin(UAnimNotify* AnimNotify)
{
	// fixme 单进程多人有bug 先这样移除
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!HasBeginNotifyList.Contains(AnimNotify))
		{
			MontageNotifyStartDelegate.Broadcast(AnimNotify);
			HasBeginNotifyList.AddUnique(AnimNotify);
		}
	}
}

void UAbilityTask_MontageNotify::AnimNotifyStateBegin(UAnimNotifyState* AnimNotifyState)
{
	// fixme 单进程多人有bug 先这样移除
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!HasBeginNotifyStateList.Contains(AnimNotifyState))
		{
			MontageNotifyStateStartDelegate.Broadcast(AnimNotifyState);
			HasBeginNotifyStateList.AddUnique(AnimNotifyState);
		}
	}
}

void UAbilityTask_MontageNotify::AnimNotifyStateEnd(UAnimNotifyState* AnimNotifyState)
{
	// fixme 单进程多人有bug 先这样移除
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!HasEndNotifyStateList.Contains(AnimNotifyState))
		{
			MontageNotifyStateEndDelegate.Broadcast(AnimNotifyState);
			HasEndNotifyStateList.AddUnique(AnimNotifyState);
		}
	}
}
