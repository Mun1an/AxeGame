// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAttributeDef.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AxeExecutionBase.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UAxeExecutionBase : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UAxeExecutionBase();

	static FAttributeDef& AttributeDefStatics()
	{
		static FAttributeDef AttributeDef;
		return AttributeDef;
	}

	void GetEvaluateParam(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                      FAggregatorEvaluateParameters& EvaluateParameters) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
