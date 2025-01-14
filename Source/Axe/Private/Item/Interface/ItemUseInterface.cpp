// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Interface/ItemUseInterface.h"

// Add default functionality here for any IItemUseInterface functions that are not pure virtual.
bool IItemUseInterface::CanUseItem()
{
	return true;
}

bool IItemUseInterface::OnUseItem()
{
	return true;
}

bool IItemUseInterface::GetIsReduceAfterUse()
{
	return true;
}
