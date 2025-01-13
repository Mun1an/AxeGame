// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/GoldCoinCountExecution.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Enum/AxeTypes.h"

UGoldCoinCountExecution::UGoldCoinCountExecution()
{
}

void UGoldCoinCountExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                          FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	FAggregatorEvaluateParameters EvaluateParameters;
	GetEvaluateParam(ExecutionParams, EvaluateParameters);

	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	float IncomingGoldCoinCount = EffectSpec.GetSetByCallerMagnitude(AxeGameplayTags.Effect_Magnitude_IncomingGoldCoinCount, false, 0.f);
	//
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAxeAttributeSet::GetIncomingGoldCoinCountAttribute(),
		EGameplayModOp::Additive,
		IncomingGoldCoinCount
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
