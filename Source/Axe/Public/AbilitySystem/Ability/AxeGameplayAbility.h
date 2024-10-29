// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AxeGameplayAbility.generated.h"


class UComboActionComponent;
class UActionCombatComponent;
class ICombatInterface;
class AAxeCharacterBase;
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
	/**
	 * Ability Props
	 */
	UFUNCTION()
	UAnimMontage* GetAbilityMontage() const { return AbilityMontage; }

	UFUNCTION()
	float GetAbilityUsingMovementSlowEffectMagnitude() const { return AbilityUsingMovementSlowEffectMagnitude; }

	UFUNCTION()
	AAxeCharacterBase* GetAxeCharacterOwner() const;

	/**
	 * InputTag
	 */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag InputTag;

	/**
	 * ActivationGroup
	 */
	EAxeAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(EAxeAbilityActivationGroup NewGroup) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(EAxeAbilityActivationGroup NewGroup);

	virtual bool CanReplaceAbilityByCondition(const UAxeGameplayAbility* NewAbility, AActor* Actor) const;

	/**
	 * virtual
	 */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled);

protected:
	/**
	 * Ability Props
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Props")
	TObjectPtr<UAnimMontage> AbilityMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Props")
	float AbilityUsingMovementSlowEffectMagnitude = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAxeAbilityActivationGroup ActivationGroup;

	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponentFromActorInfo() const;
	UFUNCTION()
	UActionCombatComponent* GetActionCombatComponent() const;
	UFUNCTION()
	UComboActionComponent* GetComboActionComponent() const;

	// Task
	void AddTask();

	// ANS
	UFUNCTION()
	void Ans_MovementSlow_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_MovementSlow_NotifyEnd(UAnimNotifyState* AnimNotifyState);

	UFUNCTION()
	void Ans_LaunchCharacter_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_LaunchCharacter_NotifyEnd(UAnimNotifyState* AnimNotifyState);

	UFUNCTION()
	void Ans_ComboInputCache_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_ComboInputCache_NotifyEnd(UAnimNotifyState* AnimNotifyState);

	UFUNCTION()
	void Ans_Combo_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_Combo_NotifyEnd(UAnimNotifyState* AnimNotifyState);
};
