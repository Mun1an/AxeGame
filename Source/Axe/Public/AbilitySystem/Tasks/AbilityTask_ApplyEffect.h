// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ApplyEffect.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UAbilityTask_ApplyEffect : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ApplyEffect* CreateApplyEffectTask(
		UGameplayAbility* OwningAbility,
		AActor* SourceActor,
		AActor* TargetActor,
		TSubclassOf<UGameplayEffect> EffectClass,
		int32 Level,
		float Duration
	);
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	UFUNCTION()
	void ApplyEffect();
	UFUNCTION(BlueprintCallable)
	void RemoveEffect();

	UPROPERTY()
	AActor* SourceActor;
	UPROPERTY()
	AActor* TargetActor;
	UPROPERTY()
	TSubclassOf<UGameplayEffect> EffectClass;
	UPROPERTY()
	int32 Level;
	UPROPERTY()
	float Duration;

	UPROPERTY(BlueprintReadOnly)
	FActiveGameplayEffectHandle EffectHandle;
};
