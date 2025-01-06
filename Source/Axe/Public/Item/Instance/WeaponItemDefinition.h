// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemDefinition.h"
#include "WeaponItemDefinition.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class AXE_API UWeaponItemDefinition : public UEquipmentItemDefinition
{
	GENERATED_BODY()

public:
	UWeaponItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
