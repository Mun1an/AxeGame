// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/WeaponItemDefinition.h"

#include "Item/Instance/WeaponItemInstance.h"

UWeaponItemDefinition::UWeaponItemDefinition(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	ItemInstanceClass = UWeaponItemInstance::StaticClass();
}
