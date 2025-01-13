// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Action/DrinkAbility.h"

#include "Character/AxeCharacterPlayer.h"

bool UDrinkAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayTagContainer* SourceTags,
                                       const FGameplayTagContainer* TargetTags,
                                       FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(AxeCharacterOwner);
	UInventoryComponent* InventoryComponent = AxeCharacterPlayer->GetInventoryComponent();
	

	return Result;
}
