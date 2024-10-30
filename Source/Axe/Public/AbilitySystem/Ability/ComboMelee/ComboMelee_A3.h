// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ComboGameplayAbility.h"
#include "ComboMelee_A3.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UComboMelee_A3 : public UComboGameplayAbility
{
	GENERATED_BODY()
	virtual void SetHitTraceDefaultValue() override;
};
