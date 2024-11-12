// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/AxeExecutionBase.h"
#include "Enum/AxeTypes.h"
#include "DamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UDamageExecution : public UAxeExecutionBase
{
	GENERATED_BODY()

public:
	UDamageExecution();


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
