// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMC_Base.h"
#include "MMC_MaxStamina.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UMMC_MaxStamina : public UMMC_Base
{
	GENERATED_BODY()

public:
	UMMC_MaxStamina();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
