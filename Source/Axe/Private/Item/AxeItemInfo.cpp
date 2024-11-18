// Fill out your copyright notice in the Description page of Project Settings.
#include "Item/AxeItemInfo.h"

bool FItemTableRowFragmentArrayHandle::FindFragment_UI(FItemTableRowFragment_UI& OutFragment)
{
	for (FInstancedStruct& InstancedStruct : Data.Get()->ItemFragments)
	{
		if (const FItemTableRowFragment_UI* Fragment = InstancedStruct.GetPtr<FItemTableRowFragment_UI>())
		{
			OutFragment = *Fragment;
			return true;
		}
	}
	return false;
}

bool FItemTableRowFragmentArrayHandle::FindFragment_World(FItemTableRowFragment_World& OutFragment)
{
	for (FInstancedStruct& InstancedStruct : Data.Get()->ItemFragments)
	{
		if (const FItemTableRowFragment_World* Fragment = InstancedStruct.GetPtr<FItemTableRowFragment_World>())
		{
			OutFragment = *Fragment;
			return true;
		}
	}
	return false;
}
