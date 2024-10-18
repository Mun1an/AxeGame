// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AxeAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UAxeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


public:
	//
	void AbilityInputTagPressed(const FGameplayTag& AbilityTag);
	void AbilityInputTagHeld(const FGameplayTag& AbilityTag);
	void AbilityInputTagReleased(const FGameplayTag& AbilityTag);

	//
	void GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability, const int32 AbilityLevel);
	
	
};
