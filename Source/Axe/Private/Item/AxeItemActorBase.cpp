// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/AxeItemActorBase.h"

#include "Item/Component/ItemComponent.h"

AAxeItemActorBase::AAxeItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
}

void AAxeItemActorBase::GetInteractionOptions(FInteractionOption& OutOptions) const
{
	OutOptions = InteractionOption;
}
