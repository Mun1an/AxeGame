// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/XpExecution.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Enum/AxeTypes.h"

UXpExecution::UXpExecution()
{
}

void UXpExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                          FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	FAggregatorEvaluateParameters EvaluateParameters;
	GetEvaluateParam(ExecutionParams, EvaluateParameters);

	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	float IncomingXpValue = EffectSpec.GetSetByCallerMagnitude(AxeGameplayTags.Effect_Magnitude_IncomingXp, false, 0.f);
	//
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAxeAttributeSet::GetIncomingXpAttribute(),
		EGameplayModOp::Additive,
		IncomingXpValue
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
