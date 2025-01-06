// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemDefinition.h"

#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"


UItemDefinition::UItemDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ItemInstanceClass = UItemInstance::StaticClass();
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
	return FindFragment<UItemFragment_CommonInfo>();
}

int32 UItemDefinition::GetItemMaxStackSize() const
{
	const UItemFragment_CommonInfo* Fragment = FindFragment<UItemFragment_CommonInfo>();
	if (Fragment)
	{
		return Fragment->ItemMaxStackSize;
	}
	return 1;
}
