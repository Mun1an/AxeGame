// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_ApplyEffect.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"

UAbilityTask_ApplyEffect* UAbilityTask_ApplyEffect::CreateApplyEffectTask(UGameplayAbility* OwningAbility,
                                                                          AActor* SourceActor, AActor* TargetActor,
                                                                          TSubclassOf<UGameplayEffect> EffectClass,
                                                                          int32 Level, float Duration)
{
	UAbilityTask_ApplyEffect* MyObj = NewAbilityTask<UAbilityTask_ApplyEffect>(OwningAbility);
	MyObj->SourceActor = SourceActor;
	MyObj->TargetActor = TargetActor;
	MyObj->EffectClass = EffectClass;
	MyObj->Level = Level;
	MyObj->Duration = Duration;

	return MyObj;
}


void UAbilityTask_ApplyEffect::Activate()
{
	Super::Activate();

	if (IsLocallyControlled())
	{
		
	}
	else
	{
		
	}
	ApplyEffect();
	
}


void UAbilityTask_ApplyEffect::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);

	RemoveEffect();
}

void UAbilityTask_ApplyEffect::ApplyEffect()
{
	const IAbilitySystemInterface* SourceASCActor = Cast<IAbilitySystemInterface>(SourceActor);
	const IAbilitySystemInterface* TargetASCActor = Cast<IAbilitySystemInterface>(TargetActor);
	check(SourceASCActor)
	check(TargetASCActor)

	UAbilitySystemComponent* SourceASC = SourceASCActor->GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = TargetASCActor->GetAbilitySystemComponent();

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		EffectClass, Level, ContextHandle
	);
	SpecHandle.Data->SetDuration(Duration, true);
	EffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), TargetASC,
		GetActivationPredictionKey()
	);
}

void UAbilityTask_ApplyEffect::RemoveEffect()
{
	if (EffectHandle.IsValid())
	{
		const IAbilitySystemInterface* SourceASCActor = Cast<IAbilitySystemInterface>(SourceActor);
		SourceASCActor->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(EffectHandle);
	}
}
