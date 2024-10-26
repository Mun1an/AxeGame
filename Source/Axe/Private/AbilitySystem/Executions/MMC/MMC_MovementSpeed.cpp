// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_MovementSpeed.h"

UMMC_MovementSpeed::UMMC_MovementSpeed()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_MovementSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float DexterityAttrValue = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, DexterityAttrValue);

	return 600 + (DexterityAttrValue * 5.f);
}
