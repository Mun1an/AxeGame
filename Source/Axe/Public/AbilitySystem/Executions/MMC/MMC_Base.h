// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "AbilitySystem/Executions/FAttributeDef.h"
#include "MMC_Base.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UMMC_Base : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Base();

	static FAttributeDef& AttributeDefStatics()
	{
		static FAttributeDef AttributeDef;
		return AttributeDef;
	}

	void GetEvaluationParams(const FGameplayEffectSpec& Spec,
	                         FAggregatorEvaluateParameters& EvaluationParameters) const;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
