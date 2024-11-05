// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MaxHealthExecution.h"

UMaxHealthExecution::UMaxHealthExecution()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
}

void UMaxHealthExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	// FAggregatorEvaluateParameters EvaluateParameters;
	// GetEvaluateParam(ExecutionParams, EvaluateParameters);
	//
	// float StrengthValue = 0.0f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
	// 	AttrDefStatics().StrengthDef, EvaluateParameters, StrengthValue
	// );
	//
	// float MaxHealth = StrengthValue * 10;
	//
	// const FGameplayModifierEvaluatedData EvaluatedData(
	// 	UAxeAttributeSet::GetMaxHealthAttribute(),
	// 	EGameplayModOp::Override,
	// 	MaxHealth
	// );
	// OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
