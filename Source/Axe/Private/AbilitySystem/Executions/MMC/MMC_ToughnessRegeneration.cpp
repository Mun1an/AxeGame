// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_ToughnessRegeneration.h"

UMMC_ToughnessRegeneration::UMMC_ToughnessRegeneration()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_ToughnessRegeneration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, Strength);
	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, Dexterity);

	return 5 + Strength * 0.1 + Dexterity * 0.1;
}
