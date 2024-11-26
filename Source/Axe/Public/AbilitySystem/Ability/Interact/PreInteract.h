// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "PreInteract.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UPreInteract : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	UPreInteract(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	UFUNCTION()
	void FindTargetInteractable();

	AActor* GetGoodTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRate = 0.1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRange = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanAngleRange = 30;
	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	UPROPERTY()
	TArray<AActor*> OutActors;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	FTimerHandle ScanTimerHandle;

	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;
};
