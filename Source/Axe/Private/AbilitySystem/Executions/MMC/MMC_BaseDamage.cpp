// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_BaseDamage.h"

UMMC_BaseDamage::UMMC_BaseDamage()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().IntelligenceDef);
}

float UMMC_BaseDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float StrengthAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, StrengthAttrValue);
	float DexterityAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, DexterityAttrValue);
	float IntelligenceAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().IntelligenceDef, Spec, EvaluationParameters,
	                              IntelligenceAttrValue);

	return 1 + StrengthAttrValue * 0.5 + DexterityAttrValue * 0.8 + IntelligenceAttrValue * 0.4;
}
