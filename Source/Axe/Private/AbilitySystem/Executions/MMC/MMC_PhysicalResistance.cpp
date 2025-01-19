// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_PhysicalResistance.h"

UMMC_PhysicalResistance::UMMC_PhysicalResistance()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().ArmorDef);
}

float UMMC_PhysicalResistance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	GetEvaluationParams(Spec, EvaluationParameters);

	float Armor = 0.f;
	GetCapturedAttributeMagnitude(AttributeDefStatics().ArmorDef, Spec, EvaluationParameters, Armor);

	float PhysicalResistance = (0.06 * Armor) / (1 + 0.06 * FMath::Abs(Armor));

	return PhysicalResistance;
}
