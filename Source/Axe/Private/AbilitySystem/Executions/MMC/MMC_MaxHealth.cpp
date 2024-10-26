// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_MaxHealth.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float StrengthAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, StrengthAttrValue);

	return 50.f + (StrengthAttrValue * 5.f);
}
