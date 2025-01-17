// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "PreSearchEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UPreSearchEnemy : public UAxeGameplayAbility
{
	GENERATED_BODY()

public:
	UPreSearchEnemy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	UFUNCTION()
	void FindTargetAndHandleEnemyInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRate = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRange = 1500;

	UPROPERTY()
	TArray<AActor*> OutActors;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	FTimerHandle ScanTimerHandle;

	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapObjectTypes;
};
