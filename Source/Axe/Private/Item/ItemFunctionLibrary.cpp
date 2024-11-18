// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunctionLibrary.h"

#include "Item/Instance/ItemInstance.h"


bool UItemFunctionLibrary::FindFragment_UI(UItemInstance* ItemInstance, FItemTableRowFragment_UI& OutFragmentStruct)
{
	if (!IsValid(ItemInstance))
	{
		return false;
	}
	FItemTableRowFragmentArrayHandle Handle = ItemInstance->GetItemStructsHandle();
	if (!Handle.Data.Get()->ItemFragments.IsEmpty())
	{
		return Handle.FindFragment_UI(OutFragmentStruct);
	}
	return false;
}

bool UItemFunctionLibrary::FindFragment_World(UItemInstance* ItemInstance,
                                              FItemTableRowFragment_World& OutFragmentStruct)
{
	if (!IsValid(ItemInstance))
	{
		return false;
	}
	FItemTableRowFragmentArrayHandle Handle = ItemInstance->GetItemStructsHandle();
	if (!Handle.Data.Get()->ItemFragments.IsEmpty())
	{
		return Handle.FindFragment_World(OutFragmentStruct);
	}
	return false;
}
