// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ItemActor/ArmorEquipmentItemActor.h"

AArmorEquipmentItemActor::AArmorEquipmentItemActor()
{
	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}
