// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunctionLibrary.h"

#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"

const UItemFragment* UItemFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef,
	TSubclassOf<UItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
