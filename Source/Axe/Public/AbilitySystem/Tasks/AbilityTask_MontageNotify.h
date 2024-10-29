// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_MontageNotify.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMontageNotifyDelegate, UAnimNotifyState*, AnimNotifyState);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMontageNotifyEndDelegate, UAnimNotifyState*, AnimNotifyState, bool, IsInterrupted);

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
	FMontageNotifyDelegate MontageNotifyTickDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FMontageNotifyDelegate MontageNotifyEndDelegate;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_MontageNotify* CreateMontageNotifyStateTask(
		UGameplayAbility* OwningAbility,
		UAnimMontage* AbilityMontage,
		TSubclassOf<UAnimNotifyState> NotifyStateClass
	);

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	UPROPERTY()
	TObjectPtr<UAnimMontage> AbilityMontage;
	UPROPERTY()
	TSubclassOf<UAnimNotifyState> NotifyStateClass;

	UPROPERTY()
	TArray<UAnimNotifyState*> NotifyStateList;

	TMap<UAnimNotifyState*, TArray<FDelegateHandle>> DelegateHandleMap;

	void AnimNotifyStateBegin(UAnimNotifyState* AnimNotifyState);
	void AnimNotifyStateTick(UAnimNotifyState* AnimNotifyState);
	void AnimNotifyStateEnd(UAnimNotifyState* AnimNotifyState);
};
