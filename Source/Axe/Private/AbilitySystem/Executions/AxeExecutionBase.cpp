// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/AxeExecutionBase.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

struct FAttrDefStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FAttrDefStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(
			UAxeAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FAttrDefStatics& AttrDefStatics()
{
	static FAttrDefStatics Statics;
	return Statics;
}

UAxeExecutionBase::UAxeExecutionBase()
{
}

void UAxeExecutionBase::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
