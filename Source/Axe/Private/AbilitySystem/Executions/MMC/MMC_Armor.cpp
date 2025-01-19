// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_Armor.h"

UMMC_Armor::UMMC_Armor()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().StrengthDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().DexterityDef);
}

float UMMC_Armor::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().StrengthDef, Spec, EvaluationParameters, Strength);
	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().DexterityDef, Spec, EvaluationParameters, Dexterity);

	return 1 + Strength * 0.1 + Dexterity * 0.1;
}
