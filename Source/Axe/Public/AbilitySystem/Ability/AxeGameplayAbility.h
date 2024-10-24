// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AxeGameplayAbility.generated.h"


class UAxeAbilitySystemComponent;

UENUM(BlueprintType)
enum class EAxeAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent,

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable,

	// 只能在条件下被Exclusive_ReplaceableByCondition或直接被Exclusive_Blocking打断和取消
	Exclusive_ReplaceableByCondition,

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking,

	Max
};

/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UAxeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag InputTag;

	EAxeAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(EAxeAbilityActivationGroup NewGroup) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(EAxeAbilityActivationGroup NewGroup);

	virtual bool CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const;
	
	//
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAxeAbilityActivationGroup ActivationGroup;

	TObjectPtr<UAxeAbilitySystemComponent> GetAxeAbilitySystemComponentFromActorInfo() const;
};
