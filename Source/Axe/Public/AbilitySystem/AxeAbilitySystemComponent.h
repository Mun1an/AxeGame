// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Ability/AxeGameplayAbility.h"
#include "AxeAbilitySystemComponent.generated.h"

class UAxeGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyAbilityActivatedDelegate, UGameplayAbility*)
/**
 * 
 */
UCLASS()
class AXE_API UAxeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAxeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	//
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	//
	void TryActivateAbilityAndCheck(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true);
	//
	void GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability, const int32 AbilityLevel);

	//
	bool IsActivationGroupBlocked(EAxeAbilityActivationGroup Group) const;
	void AddAbilityToActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);
	void RemoveAbilityFromActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);
	//
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec);
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	                                bool bWasCancelled) override;
	
	FOnNotifyAbilityActivatedDelegate OnNotifyAbilityActivatedDelegate;

protected:
	void SetLastAbilityTag(const FGameplayTag& Tag) { LastAbilityTag = Tag; }

private:
	UPROPERTY()
	FGameplayTag LastAbilityTag = FGameplayTag::EmptyTag;

	TMap<EAxeAbilityActivationGroup, TArray<UGameplayAbility*>> ActivationGroupMap;
};
