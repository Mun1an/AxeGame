// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemInstance.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (ItemStructsHandle.IsValid() && ItemStructsHandle.Data)
	{
		TagContainer.AppendTags(ItemStructsHandle.Data->ItemTags);
	}
}
