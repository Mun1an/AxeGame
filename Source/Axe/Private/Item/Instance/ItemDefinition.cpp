// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemDefinition.h"

#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"


UItemDefinition::UItemDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

const UItemFragment* UItemDefinition::FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

const UItemFragment_CommonInfo* UItemDefinition::GetCommonInfoFragment() const
{
	const UItemFragment* Fragment = FindFragmentByClass(UItemFragment_CommonInfo::StaticClass());
	if (Fragment)
	{
		const UItemFragment_CommonInfo* ItemFragment_CommonInfo = Cast<UItemFragment_CommonInfo>(Fragment);
		return ItemFragment_CommonInfo;
	}
	return nullptr;
}

int32 UItemDefinition::GetItemMaxStackSize() const
{
	const UItemFragment* Fragment = FindFragmentByClass(UItemFragment_CommonInfo::StaticClass());
	if (Fragment)
	{
		return Cast<UItemFragment_CommonInfo>(Fragment)->ItemMaxStackSize;
	}
	return 1;
}
