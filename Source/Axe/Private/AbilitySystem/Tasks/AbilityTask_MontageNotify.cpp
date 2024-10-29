// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"

#include "GameplayTasksComponent.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

UAbilityTask_MontageNotify* UAbilityTask_MontageNotify::CreateMontageNotifyStateTask(UGameplayAbility* OwningAbility,
	UAnimMontage* AbilityMontage, TSubclassOf<UAnimNotifyState> NotifyStateClass)
{
	UAbilityTask_MontageNotify* MyObj = NewAbilityTask<UAbilityTask_MontageNotify>(OwningAbility);

	MyObj->AbilityMontage = AbilityMontage;
	MyObj->NotifyStateClass = NotifyStateClass;

	return MyObj;
}

void UAbilityTask_MontageNotify::Activate()
{
	Super::Activate();

	if (!IsValid(AbilityMontage) || !IsValid(NotifyStateClass))
	{
		return;
	}

	TArray<FAnimNotifyEvent> AnimNotifyEvents = AbilityMontage->Notifies;
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
		TArray<FDelegateHandle> DelegateHandleList;
		DelegateHandleList.Add(BeginDelegateHandle);
		DelegateHandleList.Add(EndDelegateHandle);
		DelegateHandleMap.Add(AnimNotifyState, DelegateHandleList);
	}
}

void UAbilityTask_MontageNotify::OnDestroy(bool AbilityEnded)
{
	if (NotifyStateList.Num() > 0)
	{
		for (UAnimNotifyState* AnimNotifyState : NotifyStateList)
		{
			TArray<FDelegateHandle>* DelegateHandleList = DelegateHandleMap.Find(AnimNotifyState);
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
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_MontageNotify::AnimNotifyStateBegin(UAnimNotifyState* AnimNotifyState)
{
	MontageNotifyStartDelegate.Broadcast(AnimNotifyState);
}

void UAbilityTask_MontageNotify::AnimNotifyStateTick(UAnimNotifyState* AnimNotifyState)
{
	MontageNotifyTickDelegate.Broadcast(AnimNotifyState);
}

void UAbilityTask_MontageNotify::AnimNotifyStateEnd(UAnimNotifyState* AnimNotifyState)
{
	MontageNotifyEndDelegate.Broadcast(AnimNotifyState);
}
