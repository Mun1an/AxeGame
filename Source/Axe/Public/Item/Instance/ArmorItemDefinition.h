// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemDefinition.h"
#include "ArmorItemDefinition.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class AXE_API UArmorItemDefinition : public UEquipmentItemDefinition
{
	GENERATED_BODY()

public:
	UArmorItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
