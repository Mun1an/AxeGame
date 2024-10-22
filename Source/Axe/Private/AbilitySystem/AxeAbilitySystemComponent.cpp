// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "ActionSystem/ComboActionComponent.h"
#include "ActionSystem/ComboDataAsset.h"
#include "Character/AxeCharacterPlayer.h"

void UAxeAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag& AbilityTag)
{
}

void UAxeAbilitySystemComponent::AbilityInputTagHeld(FGameplayTag& AbilityTag)
{
	// TArray<FGameplayAbilitySpec> GameplayAbilitySpecList = GetActivatableAbilities();
	// for (FGameplayAbilitySpec& AbilitySpec : GameplayAbilitySpecList)
	// {
	// 	if (!AbilitySpec.IsActive() && AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag))
	// 	{
	// 		TryActivateAbilitiesByTag(AbilitySpec.Handle);
	// 	}
	// }
}

void UAxeAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag& AbilityTag)
{
	//
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetAvatarActor());
	if (IsValid(AxeCharacterPlayer))
	{
		AbilityTag = AxeCharacterPlayer->GetComboActionComponent()->GetCombo(AbilityTag);
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	TryActivateAbilitiesByTag(TagContainer);
	//
	SetLastAbilityTag(AbilityTag);
}

void UAxeAbilitySystemComponent::GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability,
                                                              const int32 AbilityLevel)
{
	const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	GiveAbility(AbilitySpec);
}
