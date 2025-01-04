// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Ability/AxeGameplayAbility.h"
#include "AxeAbilitySystemComponent.generated.h"

struct FEquipmentInfo;
struct FDamageEffectParams;
class AAxeCharacterBase;
class UAxeGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyAbilityActivatedDelegate, UGameplayAbility*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyAbilityEndedDelegate, UGameplayAbility*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityInputTagPressed, const FGameplayTag)
DECLARE_DELEGATE_OneParam(FAbilitySpecDataDelegate, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDelegate);

/**
 * 
 */
UCLASS()
class AXE_API UAxeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAxeAbilitySystemComponent();

	FOnNotifyAbilityActivatedDelegate OnNotifyAbilityActivatedDelegate;
	FOnNotifyAbilityEndedDelegate OnNotifyAbilityEndedDelegate;
	FOnAbilityInputTagPressed OnAbilityInputTagPressedDelegate;
	FAbilitiesGivenDelegate AbilitiesGivenDelegate;
	// 获取角色
	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* GetAxeCharacterOwner() const;
	// Input
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	//
	void TryActivateAbilityAndCheck_Client(FGameplayAbilitySpecHandle AbilitySpecHandle,
	                                       bool bAllowRemoteActivation = true);
	void TryActivateHitReactAbility(const FGameplayTag HitReactTag, const FHitResult& HitResult,
	                                AActor* SourceActor = nullptr);
	// Activation Group
	bool GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability, const int32 AbilityLevel);
	//
	TMap<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>> GetActivationGroupMap() const
	{
		return ActivationGroupMap;
	}

	void GetAbilitySpecHandlesByActivationGroup(TArray<FGameplayAbilitySpecHandle>& HandleArray,
	                                            EAxeAbilityActivationGroup Group) const;
	bool IsActivationGroupBlocked(EAxeAbilityActivationGroup Group, const UAxeGameplayAbility* NewAxeAbility) const;
	void AddAbilityToActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);
	void RemoveAbilityFromActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);

	// virtual
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec);
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	                                bool bWasCancelled) override;
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	// Effect
	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToSelfByClass(const TSubclassOf<UGameplayEffect>& EffectClass,
	                                                     const float Level);
	bool ApplyDamageEffect(AActor* SourceActor, AActor* TargetActor, const FDamageEffectParams& Params);

	bool ApplyEquipmentEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, FEquipmentInfo EquipmentInfo);

	//
	void ExecuteDelegateToGetAbilitySpec(const FAbilitySpecDataDelegate& Delegate);
	//
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//
	void OnCharacterASCInitOverCallback();

	UFUNCTION(BlueprintCallable)
	FGameplayAbilitySpecHandle GetAbilityHandleByAbilityTag(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCooldownTagsByAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	UFUNCTION(BlueprintCallable)
	UGameplayAbility* GetActiveAbilityByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable)
	class UDamageExecution* GetEffectDamageExecutionCDO();

protected:
	virtual void OnRep_ActivateAbilities() override;
	void TryActivateAbilitiesOnSpawn();

private:
	TMap<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>> ActivationGroupMap;

	FActiveGameplayEffectHandle EquipmentEffectHandle;
};
