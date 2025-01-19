// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/MMC/MMC_Base.h"
#include "MMC_PhysicalResistance.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UMMC_PhysicalResistance : public UMMC_Base
{
	GENERATED_BODY()
public:
	UMMC_PhysicalResistance();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
