// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AxeAbilitySystemComponent.generated.h"

class UComboTreeNode;
class UComboHandle;
/**
 * 
 */
UCLASS()
class AXE_API UAxeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//
	void AbilityInputTagPressed(FGameplayTag& AbilityTag);
	void AbilityInputTagHeld(FGameplayTag& AbilityTag);
	void AbilityInputTagReleased(FGameplayTag& AbilityTag);

	//
	void GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability, const int32 AbilityLevel);

protected:
	void SetLastAbilityTag(const FGameplayTag& Tag) { LastAbilityTag = Tag; }

private:
	UPROPERTY()
	FGameplayTag LastAbilityTag = FGameplayTag::EmptyTag;

};
