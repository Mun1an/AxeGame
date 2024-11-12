// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "ShieldBlockAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UShieldBlockAbility : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	bool GetIsBlocking() const { return bIsBlocking; }
	void SetIsBlocking(bool NewIsBlocking);

protected:
	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UPROPERTY()
	bool bIsBlocking = false;
};
