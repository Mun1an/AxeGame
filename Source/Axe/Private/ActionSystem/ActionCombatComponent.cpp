// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ActionCombatComponent.h"

#include "GameplayEffectTypes.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"

UActionCombatComponent::UActionCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


FActiveGameplayEffectHandle UActionCombatComponent::ApplyMovementSlowEffectInAbilityUse(const float Level,
	const float Duration)
{
	check(AxeAbilitySystemComponent);

	FGameplayEffectContextHandle ContextHandle = AxeAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(GetOwner());
	const FGameplayEffectSpecHandle SpecHandle = AxeAbilitySystemComponent->MakeOutgoingSpec(
		MovementSlowEffectClass, Level, ContextHandle
	);
	SpecHandle.Data->SetDuration(Duration, true);
	FActiveGameplayEffectHandle EffectHandle = AxeAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), AxeAbilitySystemComponent
	);
	MovementSlowEffectHandle = EffectHandle;

	return EffectHandle;
}

void UActionCombatComponent::RemoveMovementSlowEffectInAbilityUse()
{
	if (MovementSlowEffectHandle.IsValid())
	{
		AxeAbilitySystemComponent->RemoveActiveGameplayEffect(MovementSlowEffectHandle);
	}
}

void UActionCombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
