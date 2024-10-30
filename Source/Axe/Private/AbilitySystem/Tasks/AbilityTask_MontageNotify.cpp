// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_MontageNotify.h"

#include "GameplayTasksComponent.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Character/AxeCharacterBase.h"

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

	bool bHasAuthorityTask = HasAuthorityTask();

	for (UAnimNotifyState* AnimNotifyState : NotifyStateList)
	{
		UAxeAnimNotifyStateBase* AnimNotifyStateBase = Cast<UAxeAnimNotifyStateBase>(AnimNotifyState);
		if (!IsValid(AnimNotifyStateBase))
		{
			continue;
		}
		
		FDelegateHandle BeginDelegateHandle = AnimNotifyStateBase->LocalAnimNotifyStateBeginDelegate.AddUObject(
			this, &UAbilityTask_MontageNotify::AnimNotifyStateBegin
		);
		FDelegateHandle EndDelegateHandle = AnimNotifyStateBase->LocalAnimNotifyStateEndDelegate.AddUObject(
			this, &UAbilityTask_MontageNotify::AnimNotifyStateEnd
		);
		
		TArray<FDelegateHandle>& DelegateHandleList = DelegateHandleMap.FindOrAdd(AnimNotifyState);
		DelegateHandleList.AddUnique(BeginDelegateHandle);
		DelegateHandleList.AddUnique(EndDelegateHandle);
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
				AnimNotifyStateBase->LocalAnimNotifyStateBeginDelegate.Remove(DelegateHandle);
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

bool UAbilityTask_MontageNotify::HasAuthorityTask()
{
	UAxeGameplayAbility* AxeGameplayAbility = Cast<UAxeGameplayAbility>(GetTaskOwner());
	AAxeCharacterBase* AxeCharacterOwner = AxeGameplayAbility->GetAxeCharacterOwner();
	bool bHasAuthority = AxeCharacterOwner->HasAuthority();
	return bHasAuthority;
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
