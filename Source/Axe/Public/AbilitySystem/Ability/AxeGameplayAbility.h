// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enum/AxeEnum.h"
#include "AxeGameplayAbility.generated.h"


enum class EAbilitySkillStage : uint8;
class AAxeCharacterPlayer;
enum class ELaunchCharacterDirection : uint8;
class UAbilityTask_HitTrace;
class UComboActionComponent;
class UActionCombatComponent;
class ICombatInterface;
class AAxeCharacterBase;
class UAxeAbilitySystemComponent;

UENUM(BlueprintType)
enum class EAxeAbilityActivationPolicy : uint8
{
	None,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

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

	//
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

	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* GetAxeCharacterOwner() const;
	AAxeCharacterBase* GetAxeCharacterOwner(const FGameplayAbilityActorInfo* ActorInfo) const;

	UFUNCTION()
	bool GetIsUsingClientMovement() const { return bUseClientMovement; }

	UFUNCTION(BlueprintCallable)
	AActor* GetOrFindAutoTargetActor();

	// AbilitySkillStage

	UFUNCTION()
	EAbilitySkillStage GetCurrentAbilitySkillStage() const { return CurrentAbilitySkillStage; }

	UFUNCTION()
	void SetCurrentAbilitySkillStage(EAbilitySkillStage NewStage);
	UFUNCTION()
	void OnCurrentAbilitySkillStageChanged(EAbilitySkillStage NewStage);

	UFUNCTION()
	void OnCancelAbilityByMoveInputInBackSwing(FVector LastMoveInput);
	/**
	 * InputTag
	 */
	UPROPERTY(EditDefaultsOnly, Category="Axe Ability | Input")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, Category="Axe Ability | Input")
	FGameplayTagContainer InputTagContainer;

	/**
	 * ActivationGroup
	 */
	EAxeAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EAxeAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Axe Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(EAxeAbilityActivationGroup NewGroup) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Axe Ability",
		Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(EAxeAbilityActivationGroup NewGroup);

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	virtual void HandleTryActivateAbilityClientCDO(FGameplayAbilitySpecHandle& AbilitySpecHandle, const FGameplayTag AbilityInputTag, AActor* AbilityActor);

	virtual bool CanActivateAbility_ByNowAbilityReplaceCondition(const FGameplayAbilitySpecHandle Handle,
	                                                             const FGameplayAbilityActorInfo* ActorInfo,
	                                                             const FGameplayTagContainer* SourceTags,
	                                                             const FGameplayTagContainer* TargetTags,
	                                                             FGameplayTagContainer* OptionalRelevantTags) const;

	virtual bool CanReplaceByNewAbility(const UGameplayAbility* NewAbilityCDO) const;
	/**
	 *  base virtual
	 */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

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
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	FGameplayTagContainer GetCancelAbilitiesTag() const;

protected:
	/**
	 * Ability Props
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Axe Ability | Props")
	TObjectPtr<UAnimMontage> AbilityMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Axe Ability | Activation")
	EAxeAbilityActivationGroup ActivationGroup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Axe Ability | Activation")
	EAxeAbilityActivationPolicy ActivationPolicy;

	// ClientMovement 
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | Client")
	bool bUseClientMovement = false;
	// Ability Target
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | Target")
	bool bNeedAutoSearchTarget = false;
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | Target")
	float AutoSearchTargetRadius = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | Target")
	float AutoSearchTargetAngle = 80.f;
	UPROPERTY()
	TObjectPtr<AActor> AutoTargetActor;

	// Skill Stage
	UPROPERTY()
	EAbilitySkillStage CurrentAbilitySkillStage = EAbilitySkillStage::ASS_FrontSwing;

	// 可以在后摇时被替换
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | SkillStage")
	bool bCanReplacedInBackSwing = true;
	UPROPERTY(EditDefaultsOnly, Category = "Axe Ability | SkillStage")
	bool bCanCancelByMoveInputInBackSwing = true;
	//
	UFUNCTION()
	UAxeAbilitySystemComponent* GetAxeAbilitySystemComponentFromActorInfo() const;
	UFUNCTION()
	UActionCombatComponent* GetActionCombatComponent() const;
	UFUNCTION()
	UComboActionComponent* GetComboActionComponent() const;

	// Montage PauseFrame
	UPROPERTY(EditDefaultsOnly, Category = "Axe AnimMontage|PauseFrame")
	float ActiveMontagePauseFrameRateMagnitude = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Axe AnimMontage|PauseFrame")
	float ActiveMontagePauseFrameDurationMagnitude = 1.0f;
	UFUNCTION(BlueprintCallable)
	void SetActiveMontagePauseFrame(float LowRate, float Duration);
	FTimerHandle ActiveMontagePauseFrameTimerHandle;
	FAnimMontageInstance* ActiveMontagePauseFrameInstance;
	void ActiveMontagePauseFrameEnd();

	// Camera
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	UFUNCTION(BlueprintCallable)
	void ShakeCamera();

	// Task
	UFUNCTION(BlueprintCallable)
	void AddMontageNotifyStateTask(UAnimMontage* LocalAnimMontage);
	UFUNCTION(BlueprintCallable)
	void InitMontageNotifyAndNotifyStateClasses(UAnimMontage* LocalAnimMontage);
	UPROPERTY()
	TArray<TSubclassOf<UAnimNotify>> MontageNotifyClassesCache;
	TArray<TSubclassOf<UAnimNotifyState>> MontageNotifyStateClassesCache;

	// Movement
	UFUNCTION(BlueprintCallable)
	void SetIgnoreClientMovementErrorChecksAndCorrection(bool bIsIgnore);

	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* FindOneGoodTargetByMoveInput(float SphereRadius = 300,
	                                                float TraceAngleRange = 60.f);
	UFUNCTION(BlueprintCallable)
	void SetAbilityIgnoreMoveInput(bool bIsIgnore);
	UFUNCTION(BlueprintCallable)
	void ResetAbilityIgnoreMoveInput();
	int32 AbilityIgnoreMoveInputCount = 0;

	UFUNCTION()
	void An_AbilitySkillState_NotifyBegin(UAnimNotify* AnimNotify);

	UFUNCTION()
	void An_CustomName_NotifyBegin(UAnimNotify* AnimNotify);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCustomNameNotifyBegin(FName CustomName);

	UFUNCTION()
	virtual void Ans_CustomName_NotifyBegin(UAnimNotifyState* AnimNotifyState);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCustomNameAnsBegin(FName CustomName);
	UFUNCTION()
	virtual void Ans_CustomName_NotifyEnd(UAnimNotifyState* AnimNotifyState);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCustomNameAnsEnd(FName CustomName);

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
