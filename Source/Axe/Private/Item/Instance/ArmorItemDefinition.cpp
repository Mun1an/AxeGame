// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ArmorItemDefinition.h"

#include "Item/Instance/ArmorItemInstance.h"

UArmorItemDefinition::UArmorItemDefinition(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	ItemInstanceClass = UArmorItemInstance::StaticClass();
}
