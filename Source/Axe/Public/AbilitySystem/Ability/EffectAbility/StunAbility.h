// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "StunAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UStunAbility : public UAxeGameplayAbility
{
	GENERATED_BODY()
public:
	UStunAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
