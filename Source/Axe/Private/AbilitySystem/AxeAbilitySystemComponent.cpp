// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeAbilitySystemComponent.h"

void UAxeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& AbilityTag)
{
}

void UAxeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& AbilityTag)
{
	TArray<FGameplayAbilitySpec> GameplayAbilitySpecList = GetActivatableAbilities();
	for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	{
		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAxeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& AbilityTag)
{
}

void UAxeAbilitySystemComponent::GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability,
                                                              const int32 AbilityLevel)
{
	const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	GiveAbility(AbilitySpec);
}
