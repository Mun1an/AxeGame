// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_OnDamageExecution.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "Character/AxeCharacterPlayer.h"

UAbilityTask_OnDamageExecution* UAbilityTask_OnDamageExecution::CreateOnDoDamageExecutionTask(
	UGameplayAbility* OwningAbility, AActor* DoDamageActor)
{
	UAbilityTask_OnDamageExecution* MyObj = NewAbilityTask<UAbilityTask_OnDamageExecution>(OwningAbility);
	MyObj->DoDamageActor = DoDamageActor;
	return MyObj;
}

UAbilityTask_OnDamageExecution* UAbilityTask_OnDamageExecution::CreateOnBeDamagedExecutionTask(
	UGameplayAbility* OwningAbility, AActor* BeDamagedActor)
{
	UAbilityTask_OnDamageExecution* MyObj = NewAbilityTask<UAbilityTask_OnDamageExecution>(OwningAbility);
	MyObj->BeDamagedActor = BeDamagedActor;
	return MyObj;
}

void UAbilityTask_OnDamageExecution::Activate()
{
	Super::Activate();
	InitDamageExecutionCDO();
	if (DamageExecutionCDO)
	{
		DamageExecutionCDO->OnCalDamageDelegate.AddDynamic(this, &ThisClass::OnCalDamage);
	}
}

void UAbilityTask_OnDamageExecution::OnDestroy(bool bInOwnerFinished)
{
	if (DamageExecutionCDO)
	{
		DamageExecutionCDO->OnCalDamageDelegate.RemoveDynamic(this, &ThisClass::OnCalDamage);
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_OnDamageExecution::OnCalDamage(UDamageCalInfo* DamageCalInfo)
{
	if (DamageCalInfo->DamageSourceActor == DoDamageActor)
	{
		OnDoDamageExecutionDelegate.Broadcast(DamageCalInfo);
	}
	if (DamageCalInfo->DamageTargetActor == BeDamagedActor)
	{
		OnBeDamagedExecutionDelegate.Broadcast(DamageCalInfo);
	}
}

void UAbilityTask_OnDamageExecution::InitDamageExecutionCDO()
{
	if (DamageExecutionCDO == nullptr)
	{
		AActor* OwnerActor = DoDamageActor != nullptr ? DoDamageActor : BeDamagedActor;
		UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(OwnerActor)->GetAbilitySystemComponent();
		if (ASC)
		{
			UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
			DamageExecutionCDO = AxeASC->GetEffectDamageExecution();
		}
	}
}
