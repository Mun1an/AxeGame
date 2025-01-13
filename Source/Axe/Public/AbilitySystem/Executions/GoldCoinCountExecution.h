// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/AxeExecutionBase.h"
#include "GoldCoinCountExecution.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UGoldCoinCountExecution : public UAxeExecutionBase
{
	GENERATED_BODY()

public:
	UGoldCoinCountExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;


};
