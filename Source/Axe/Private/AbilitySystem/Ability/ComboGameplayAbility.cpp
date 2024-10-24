// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ComboGameplayAbility.h"

#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"

bool UComboGameplayAbility::CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const
{
	if (!Super::CanReplaceAbilityByCondition(NewAbility, Actor))
	{
		return false;
	}

	if (AAxeCharacterPlayer* CharacterPlayer = Cast<AAxeCharacterPlayer>(Actor))
	{
		UComboActionComponent* ComboActionComponent = CharacterPlayer->GetComboActionComponent();
		bool bIsNextComboAbility = ComboActionComponent->IsNextComboAbility(NewAbility);
		bool bIsInComboSwitchWindow = ComboActionComponent->IsInComboSwitchWindow();

		return bIsNextComboAbility && bIsInComboSwitchWindow;
	}
	return true;
}
