// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Executions/MMC/MMC_Base.h"
#include "MMC_Armor.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UMMC_Armor : public UMMC_Base
{
	GENERATED_BODY()
public:
	UMMC_Armor();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
