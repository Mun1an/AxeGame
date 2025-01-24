// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/EffectComponent/AddStunEffectComponent.h"

#include "GameplayEffect.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "GameplayTag/AxeGameplayTags.h"

UAddStunEffectComponent::UAddStunEffectComponent()
{
}

bool UAddStunEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                          FActiveGameplayEffect& ActiveGE) const
{
	bool bResult = Super::OnActiveGameplayEffectAdded(ActiveGEContainer, ActiveGE);

	if (ActiveGEContainer.IsNetAuthority())
	{
		ActiveGE.EventSet.OnEffectRemoved.AddUObject(this, &UAddStunEffectComponent::OnActiveGameplayEffectRemoved);
		ActiveGE.EventSet.OnInhibitionChanged.AddUObject(this, &UAddStunEffectComponent::OnInhibitionChanged);
	}
	return bResult;
}

void UAddStunEffectComponent::OnInhibitionChanged(FActiveGameplayEffectHandle ActiveGEHandle, bool bIsInhibited) const
{
	if (bIsInhibited)
	{
		EndAbilities(ActiveGEHandle);
	}
	else
	{
		StartAbilities(ActiveGEHandle);
	}
}

void UAddStunEffectComponent::OnActiveGameplayEffectRemoved(const FGameplayEffectRemovalInfo& RemovalInfo) const
{
	const FActiveGameplayEffect* ActiveGE = RemovalInfo.ActiveEffect;
	if (!ensure(ActiveGE))
	{
		return;
	}
	EndAbilities(ActiveGE->Handle);
}

void UAddStunEffectComponent::StartAbilities(FActiveGameplayEffectHandle ActiveGEHandle) const
{
	UAbilitySystemComponent* ASC = ActiveGEHandle.GetOwningAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	const FGameplayTagContainer TagContainer = FGameplayTagContainer(FAxeGameplayTags::Get().Ability_Stun);
	AxeASC->TryActivateAbilitiesByTag(TagContainer);
}

void UAddStunEffectComponent::EndAbilities(FActiveGameplayEffectHandle ActiveGEHandle) const
{
	UAbilitySystemComponent* ASC = ActiveGEHandle.GetOwningAbilitySystemComponent();

	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	FGameplayAbilitySpecHandle AbilityHandleByAbilityTag = AxeASC->GetAbilityHandleByAbilityTag(
		FAxeGameplayTags::Get().Ability_Stun);
	AxeASC->CancelAbilityHandle(AbilityHandleByAbilityTag);
}
