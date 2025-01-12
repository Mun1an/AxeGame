// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/AxeExecutionBase.h"
#include "XpExecution.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UXpExecution : public UAxeExecutionBase
{
	GENERATED_BODY()

public:
	UXpExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

};
