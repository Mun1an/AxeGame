// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_World.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UItemComponent::SetItemInstance(UItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
	ItemDef = ItemInstance->GetItemDef();
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!ItemInstance && ItemDef)
	{
		ItemInstance = UItemFunctionLibrary::CreateItemInstance(GetWorld(), ItemDef);
	}
}
