// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AxeGameplayAbility.generated.h"


enum class ELaunchCharacterDirection : uint8;
class UAbilityTask_HitTrace;
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
	
	UFUNCTION()
	bool GetIsUsingClientMovement() const { return bUseClientMovement; }
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
	 *  base virtual
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Props")
	TObjectPtr<UAnimMontage> AbilityMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Props")
	float AbilityUsingMovementSlowEffectMagnitude = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAxeAbilityActivationGroup ActivationGroup;

	UPROPERTY(EditDefaultsOnly)
	bool bUseClientMovement = true;

	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponentFromActorInfo() const;
	UFUNCTION()
	UActionCombatComponent* GetActionCombatComponent() const;
	UFUNCTION()
	UComboActionComponent* GetComboActionComponent() const;

	// Montage PauseFrame
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|PauseFrame")
	float ActiveMontagePauseFrameRateMagnitude = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|PauseFrame")
	float ActiveMontagePauseFrameDurationMagnitude = 1.0f;
	UFUNCTION(BlueprintCallable)
	void SetActiveMontagePauseFrame(float LowRate, float Duration);
	FTimerHandle ActiveMontagePauseFrameTimerHandle;
	FAnimMontageInstance* ActiveMontagePauseFrameInstance;
	void ActiveMontagePauseFrameEnd();

	// Camera
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	void ShakeCamera();

	// Task
	UFUNCTION(BlueprintCallable)
	void AddMontageNotifyStateTask(UAnimMontage* LocalAnimMontage);

	// Movement
	UFUNCTION(BlueprintCallable)
	void SetIgnoreClientMovementErrorChecksAndCorrection(bool bIsIgnore);

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
	void Ans_IgnoreInput_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_IgnoreInput_NotifyEnd(UAnimNotifyState* AnimNotifyState);
	
	UFUNCTION()
	void Ans_MotionWrap_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION()
	void Ans_MotionWrap_NotifyEnd(UAnimNotifyState* AnimNotifyState);
};
