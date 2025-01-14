// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/UseItemDefinition.h"
#include "Item/Instance/UseItemInstance.h"

UUseItemDefinition::UUseItemDefinition(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	ItemInstanceClass = UUseItemInstance::StaticClass();
}
