// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/AxeExecutionBase.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"


UAxeExecutionBase::UAxeExecutionBase()
{
}

void UAxeExecutionBase::GetEvaluateParam(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                           FAggregatorEvaluateParameters& EvaluateParameters) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
}

void UAxeExecutionBase::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
