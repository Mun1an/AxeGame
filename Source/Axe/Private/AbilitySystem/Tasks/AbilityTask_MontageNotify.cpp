// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"

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

	if (AbilityMontage && NotifyStateClass)
	{
		TArray<FAnimNotifyEvent> AnimNotifyEvents = AbilityMontage->Notifies;
		for (FAnimNotifyEvent& AnimNotifyEvent : AnimNotifyEvents)
		{
			if (AnimNotifyEvent.NotifyStateClass.GetClass() == NotifyStateClass)
			{
				NotifyStateList.Add(AnimNotifyEvent.NotifyStateClass);
			}
		}
	}
	if (NotifyStateList.Num() > 0)
	{
		for (UAnimNotifyState* AnimNotifyState : NotifyStateList)
		{
			UAxeAnimNotifyStateBase* AnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(AnimNotifyState);
			if (AnimNotifyStateBase)
			{
				FDelegateHandle BeginDelegateHandle = AnimNotifyStateBase->AnimNotifyStateBeginDelegate.AddLambda(
					[this]
					{
						this->MontageNotifyStartDelegate.Broadcast();
					}
				);
				FDelegateHandle TickDelegateHandle = AnimNotifyStateBase->AnimNotifyStateTickDelegate.AddLambda(
					[this]
					{
						this->MontageNotifyTickDelegate.Broadcast();
					}
				);
				FDelegateHandle EndDelegateHandle = AnimNotifyStateBase->AnimNotifyStateEndDelegate.AddLambda(
					[this]
					{
						this->MontageNotifyEndDelegate.Broadcast();
					}
				);
				TArray<FDelegateHandle> DelegateHandleList;
				DelegateHandleList.Add(BeginDelegateHandle);
				DelegateHandleList.Add(TickDelegateHandle);
				DelegateHandleList.Add(EndDelegateHandle);
				DelegateHandleMap.Add(AnimNotifyState, DelegateHandleList);
			}
		}
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
			if (AnimNotifyStateBase)
			{
				for (FDelegateHandle DelegateHandle : *DelegateHandleList)
				{
					AnimNotifyStateBase->AnimNotifyStateBeginDelegate.Remove(DelegateHandle);
				}
			}
		}
	}

	Super::OnDestroy(AbilityEnded);
}
