// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemDefinition.h"

#include "Item/ItemFragment/ItemFragment.h"


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

