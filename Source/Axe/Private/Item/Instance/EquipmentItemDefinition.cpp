// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/EquipmentItemDefinition.h"

#include "Item/Instance/EquipmentItemInstance.h"

UEquipmentItemDefinition::UEquipmentItemDefinition(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	ItemInstanceClass = UEquipmentItemInstance::StaticClass();
}
