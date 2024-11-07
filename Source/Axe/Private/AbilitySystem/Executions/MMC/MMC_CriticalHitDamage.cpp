// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_CriticalHitDamage.h"

UMMC_CriticalHitDamage::UMMC_CriticalHitDamage()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_CriticalHitDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float DexterityAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, DexterityAttrValue);

	return 1.f + (DexterityAttrValue * 0.05f);
}
