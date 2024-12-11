// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/MMC/MMC_MovementSpeed.h"

#include "Character/AxeCharacterBase.h"

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

	float DefaultWalkSpeed = 100;

	UObject* SourceObject = Spec.GetContext().GetSourceObject();
	if (const AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(SourceObject))
	{
		DefaultWalkSpeed = AxeCharacterBase->GetDefaultWalkSpeed();
	}

	return DefaultWalkSpeed + (DexterityAttrValue * 5.f);
}
