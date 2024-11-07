// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_CriticalHitChance.h"

UMMC_CriticalHitChance::UMMC_CriticalHitChance()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_CriticalHitChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float DexterityAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, DexterityAttrValue);

	return 0.05f + (DexterityAttrValue * 0.005f);
}
