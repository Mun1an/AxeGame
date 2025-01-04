// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/AxeExecutionBase.h"
#include "Enum/AxeTypes.h"
#include "DamageExecution.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalDamageExecutionDelegate, UDamageCalInfo*, DamageCal);

UCLASS(BlueprintType)
class AXE_API UDamageCalInfo : public UObject
{
	GENERATED_BODY()

	UDamageCalInfo()
	{
	}

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> DamageSourceActor;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> DamageTargetActor;

	UPROPERTY(BlueprintReadWrite)
	float Damage = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBlocked = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsParried = false;
};

/**
 * 
 */
UCLASS()
class AXE_API UDamageExecution : public UAxeExecutionBase
{
	GENERATED_BODY()

public:
	UDamageExecution();

	UPROPERTY(BlueprintAssignable)
	FOnCalDamageExecutionDelegate OnCalDamageDelegate;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	void CalDamageByResistance(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                           FAggregatorEvaluateParameters EvaluateParameters, FGameplayEffectSpec EffectSpec,
	                           float& Damage) const;
	void CalCritical(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                 FAggregatorEvaluateParameters EvaluateParameters, FAxeGameplayEffectContext* AxeEffectContext,
	                 float& Damage) const;
	void CalBlocked(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                FAggregatorEvaluateParameters EvaluateParameters, FAxeGameplayEffectContext* AxeEffectContext,
	                float& Damage) const;
};
