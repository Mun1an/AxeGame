// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Ability/AxeGameplayAbility.h"
#include "AxeAbilitySystemComponent.generated.h"

struct FDamageEffectParams;
class AAxeCharacterBase;
class UAxeGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyAbilityActivatedDelegate, UGameplayAbility*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifyAbilityEndedDelegate, UGameplayAbility*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityInputTagPressed, const FGameplayTag)
/**
 * 
 */
UCLASS()
class AXE_API UAxeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAxeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FOnNotifyAbilityActivatedDelegate OnNotifyAbilityActivatedDelegate;
	FOnNotifyAbilityEndedDelegate OnNotifyAbilityEndedDelegate;
	FOnAbilityInputTagPressed OnAbilityInputTagPressedDelegate;
	//
	// 获取角色
	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* GetAxeCharacterOwner() const;
	//
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	//
	void TryActivateAbilityAndCheck_Client(FGameplayAbilitySpecHandle AbilitySpecHandle,
	                                       bool bAllowRemoteActivation = true);
	//
	void TryActivateHitReactAbility(const FGameplayTag HitReactTag, const FHitResult& HitResult,
	                                AActor* SourceActor = nullptr);
	//
	bool GiveAbilityByAbilityAndLevel(const TSubclassOf<UGameplayAbility>& Ability, const int32 AbilityLevel);
	//
	TMap<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>> GetActivationGroupMap() const
	{
		return ActivationGroupMap;
	}

	void GetAbilitySpecHandlesByActivationGroup(TArray<FGameplayAbilitySpecHandle>& HandleArray,
	                                            EAxeAbilityActivationGroup Group) const;

	// Activation Group
	bool IsActivationGroupBlocked(EAxeAbilityActivationGroup Group, const UAxeGameplayAbility* NewAxeAbility) const;
	void AddAbilityToActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);
	void RemoveAbilityFromActivationGroup(EAxeAbilityActivationGroup Group, UAxeGameplayAbility* AxeAbility);

	// virtual
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec);
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
	                                bool bWasCancelled) override;
	// Effect
	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToSelfByClass(const TSubclassOf<UGameplayEffect>& EffectClass,
	                                                     const float Level);
	bool ApplyDamageEffectToTarget(UAbilitySystemComponent* TargetASC, const FDamageEffectParams& Params);
	bool ApplyDamageEffectToSelf(AActor* FromTarget, const FDamageEffectParams& Params);

	//
protected:

private:
	TMap<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>> ActivationGroupMap;
};
