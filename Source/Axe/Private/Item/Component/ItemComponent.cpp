// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Item/AxeItemSubsystem.h"
#include "Item/Instance/ItemInstance.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Actor = GetOwner())
	{
		if (UGameInstance* GameInstance = Actor->GetGameInstance())
		{
			if (UAxeItemSubsystem* ItemSubsystem = GameInstance->GetSubsystem<UAxeItemSubsystem>())
			{
				FAxeItemTableRowDefine ItemRow;
				ItemSubsystem->FindItemDefRowDefine(ItemID, ItemRow);
				CustomItemInfo = ItemRow.ItemInfos;

				ItemInstance = ItemSubsystem->CreateItemInstance(CustomItemInfo, this);
				ItemInstance->StackSize = StackSize;
			}
		}
	}
}
