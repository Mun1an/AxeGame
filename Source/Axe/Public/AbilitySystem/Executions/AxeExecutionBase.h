// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
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

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
