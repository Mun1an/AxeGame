// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_MaxStamina.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, Strength);
	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, Dexterity);

	return 50.f + (Strength * 2.f) + (Dexterity * 2.f);
}
