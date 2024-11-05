// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/DamageExecution.h"

#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"

UDamageExecution::UDamageExecution()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().MagicResistanceDef);
}


void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	FAggregatorEvaluateParameters EvaluateParameters;
	GetEvaluateParam(ExecutionParams, EvaluateParameters);
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();
	//
	float Damage = 0.f;
	//
	CalDamageByResistance(ExecutionParams, EvaluateParameters, EffectSpec, Damage);

	//
	const FGameplayModifierEvaluatedData EvaluatedData(
		UAxeAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}


void UDamageExecution::CalDamageByResistance(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FAggregatorEvaluateParameters EvaluateParameters,
                                             const FGameplayEffectSpec EffectSpec, float& Damage) const
{
	FAxeGameplayTags& AxeGameplayTags = FAxeGameplayTags::Get();
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : AxeGameplayTags.DamageTypesToResistances)
	{
		FGameplayTag DamageTypeTag = Pair.Key;
		FGameplayTag DamageResistanceTypeTag = Pair.Value;
		float TypeDamage = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false, 0.f);

		FGameplayEffectAttributeCaptureDefinition ResistanceAttrCaptureDef;
		if (DamageResistanceTypeTag == AxeGameplayTags.Attributes_Secondary_PhysicalResistance)
		{
			ResistanceAttrCaptureDef = AttributeDefStatics().PhysicalResistanceDef;
		}
		else if (DamageResistanceTypeTag == AxeGameplayTags.Attributes_Secondary_MagicResistance)
		{
			ResistanceAttrCaptureDef = AttributeDefStatics().MagicResistanceDef;
		}
		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			ResistanceAttrCaptureDef, EvaluateParameters, ResistanceValue);

		float AddDamage = FMath::Max(0.f, TypeDamage * (1 - ResistanceValue));
		Damage += AddDamage;
	}
}
