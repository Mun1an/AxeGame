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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAxeGameplayAbility> ShieldParryAbilityClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void SetIsBlocking(bool NewIsBlocking);

	UFUNCTION(BlueprintCallable)
	void SetIsPrepareParry(bool NewIsPrepareParry);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsBlocking = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsPrepareParry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrePareParryTime = 1.0f;
	UFUNCTION()
	void OnPrePareParryFinished();

	UFUNCTION()
	void OnInputReleased(float TimeHeld);
	UFUNCTION()
	void OnEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
	                     FActiveGameplayEffectHandle ActiveHandle);
	UFUNCTION()
	void OnIncomingDamageEffectApplied(AActor* Source, FGameplayEffectSpecHandle SpecHandle,
	                                   FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION()
	void TransformToShieldParry(AActor* Source);
};
