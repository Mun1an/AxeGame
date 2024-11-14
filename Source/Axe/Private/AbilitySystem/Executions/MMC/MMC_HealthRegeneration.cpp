// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_HealthRegeneration.h"

UMMC_HealthRegeneration::UMMC_HealthRegeneration()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
}

float UMMC_HealthRegeneration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters,
	                              Strength);
	return Strength * 0.05;
}
