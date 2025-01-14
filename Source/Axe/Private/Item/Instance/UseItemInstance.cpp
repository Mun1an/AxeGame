// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/UseItemInstance.h"

bool UUseItemInstance::CanUseItem()
{
	return true;
}

bool UUseItemInstance::OnUseItem()
{
	return true;
}

bool UUseItemInstance::GetIsReduceAfterUse()
{
	return true;
}
