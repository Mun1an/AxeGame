// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ItemFragment.h"
#include "Templates/SubclassOf.h"

#include "ItemFragment_EquippableItem.generated.h"

class UEquipmentItemDefinition;

UCLASS()
class UItemFragment_EquippableItem : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<UEquipmentItemDefinition> EquipmentDefinition;
};
