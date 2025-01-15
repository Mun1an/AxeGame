// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_MaxToughness.h"

UMMC_MaxToughness::UMMC_MaxToughness()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_MaxToughness::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, Strength);
	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, Dexterity);

	return 30.f + (Strength * 0.2f) + (Dexterity * 0.2f);
}
