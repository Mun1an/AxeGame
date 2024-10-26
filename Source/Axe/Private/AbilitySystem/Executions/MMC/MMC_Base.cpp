// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_Base.h"


UMMC_Base::UMMC_Base()
{
}

void UMMC_Base::GetEvaluationParams(const FGameplayEffectSpec& Spec,
                                    FAggregatorEvaluateParameters& EvaluationParameters) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
}

float UMMC_Base::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return 0.f;
}
