// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_MontageNotify.generated.h"


class AAxeCharacterBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMontageNotifyDelegate, UAnimNotify*, AnimNotify);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMontageNotifyStateDelegate, UAnimNotifyState*, AnimNotifyState);

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_MontageNotify : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMontageNotifyDelegate MontageNotifyStartDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FMontageNotifyStateDelegate MontageNotifyStateStartDelegate;

	UPROPERTY(BlueprintAssignable)
	FMontageNotifyStateDelegate MontageNotifyStateEndDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_MontageNotify* CreateMontageNotifyStateTask(
		UGameplayAbility* OwningAbility,
		UAnimMontage* AbilityMontage,
		TSubclassOf<UAnimNotifyState> NotifyStateClass
	);

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_MontageNotify* CreateMontageNotifyTask(
		UGameplayAbility* OwningAbility,
		UAnimMontage* AbilityMontage,
		TSubclassOf<UAnimNotify> NotifyClass
	);

	void InitTaskAuthority();

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	bool bHasAuthority = false;

	UPROPERTY()
	TObjectPtr<AAxeCharacterBase> AxeCharacter;

	UPROPERTY()
	TObjectPtr<UAnimMontage> AbilityMontage;
	UPROPERTY()
	TSubclassOf<UAnimNotifyState> NotifyStateClass;
	UPROPERTY()
	TSubclassOf<UAnimNotify> NotifyClass;

	// HandleNotify
	void NotifyActivate(TArray<FAnimNotifyEvent> AnimNotifyEvents);
	void NotifyEnd();
	UPROPERTY()
	TArray<UAnimNotify*> NotifyList;
	TMap<TObjectPtr<UAnimNotify>, TArray<FDelegateHandle>> AN_DelegateHandleMap;
	void AnimNotifyBegin(UAnimNotify* AnimNotify);
	TArray<UAnimNotify*> HasBeginNotifyList;
	
	// HandleNotifyState
	void NotifyStateActivate(TArray<FAnimNotifyEvent> AnimNotifyEvents);
	void NotifyStateEnd();
	UPROPERTY()
	TArray<UAnimNotifyState*> NotifyStateList;
	TMap<TObjectPtr<UAnimNotifyState>, TArray<FDelegateHandle>> ANS_DelegateHandleMap;
	void AnimNotifyStateBegin(UAnimNotifyState* AnimNotifyState);
	void AnimNotifyStateEnd(UAnimNotifyState* AnimNotifyState);
	TArray<UAnimNotifyState*> HasBeginNotifyStateList;
	TArray<UAnimNotifyState*> HasEndNotifyStateList;
};
