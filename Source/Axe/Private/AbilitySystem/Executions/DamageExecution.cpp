// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Executions/DamageExecution.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "Character/AxeCharacterBase.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Enum/AxeTypes.h"

UDamageExecution::UDamageExecution()
{
	RelevantAttributesToCapture.Add(AttributeDefStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(AttributeDefStatics().CriticalHitDamageDef);
}


void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	FAggregatorEvaluateParameters EvaluateParameters;
	GetEvaluateParam(ExecutionParams, EvaluateParameters);

	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContext* EffectContext = EffectSpec.GetContext().Get();
	FAxeGameplayEffectContext* AxeEffectContext = UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
		EffectContext
	);
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAxeAbilitySystemComponent* TargetAxeASC = Cast<UAxeAbilitySystemComponent>(TargetASC);
	AAxeCharacterBase* TargetCharacter = TargetAxeASC->GetAxeCharacterOwner();

	FAxeGameplayTags& AxeGameplayTags = FAxeGameplayTags::Get();
	//
	float Damage = 0.f;
	//
	CalDamageByResistance(ExecutionParams, EvaluateParameters, EffectSpec, Damage);
	//
	CalCritical(ExecutionParams, EvaluateParameters, AxeEffectContext, Damage);
	//
	AxeEffectContext->SetPreDamageValue(Damage);

	// Broadcast
	UDamageCalInfo* DamageCal = NewObject<UDamageCalInfo>();
	DamageCal->DamageSourceActor = Cast<AActor>(AxeEffectContext->GetSourceObject());
	DamageCal->DamageTargetActor = TargetCharacter;
	DamageCal->Damage = Damage;

	OnCalDamageDelegate.Broadcast(DamageCal);

	Damage = DamageCal->Damage;
	if (DamageCal->bIsBlocked)
	{
		AxeEffectContext->SetDamageSpecialExpression(EDamageSpecialExpression::BeBlocked);
	}
	if (DamageCal->bIsParried)
	{
		AxeEffectContext->SetDamageSpecialExpression(EDamageSpecialExpression::BeParried);
	}
	if (DamageCal->bIsEvaded)
	{
		AxeEffectContext->SetDamageSpecialExpression(EDamageSpecialExpression::BeEvaded);
	}

	// TODO
	AxeEffectContext->SetDamageType(AxeGameplayTags.Damage_Physical);
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

void UDamageExecution::CalCritical(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                   FAggregatorEvaluateParameters EvaluateParameters,
                                   FAxeGameplayEffectContext* AxeEffectContext, float& Damage) const
{
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AttributeDefStatics().CriticalHitChanceDef, EvaluateParameters, CriticalHitChance);
	float CriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AttributeDefStatics().CriticalHitDamageDef, EvaluateParameters, CriticalHitDamage);
	const bool bIsCritical = FMath::RandRange(0.f, 1.f) < CriticalHitChance;
	float DamageCoefficient = 1.f;
	if (bIsCritical)
	{
		DamageCoefficient = DamageCoefficient * CriticalHitDamage;
		AxeEffectContext->SetDamageSpecialExpression(EDamageSpecialExpression::CriticalHit);
	}
	Damage = Damage * DamageCoefficient;
}
