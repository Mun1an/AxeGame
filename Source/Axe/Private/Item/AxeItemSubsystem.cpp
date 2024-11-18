// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/AxeItemSubsystem.h"

#include "Item/AxeItemInfo.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Settings/AxeItemSettings.h"

bool UAxeItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UAxeItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 加载ItemDefDataTable
	const UAxeItemSettings* AxeItemSettings = GetDefault<UAxeItemSettings>();
	check(AxeItemSettings->ItemDefDataTable);
	UDataTable* DataTable = AxeItemSettings->ItemDefDataTable.LoadSynchronous();
}

void UAxeItemSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UItemInstance* UAxeItemSubsystem::CreateItemInstance(const FItemTableRowFragmentArray& ItemInfo, UObject* Outer)
{
	if (ItemInfo.ItemFragments.IsEmpty())
	{
		return nullptr;
	}
	FItemTableRowFragmentArrayHandle FragmentArrayHandle = FItemTableRowFragmentArrayHandle(
		new FItemTableRowFragmentArray(ItemInfo));
	UItemInstance* ItemInstance = NewObject<UItemInstance>(Outer);
	ItemInstance->SetItemStructsHandle(FragmentArrayHandle);
	return ItemInstance;
}

UDataTable* UAxeItemSubsystem::GetItemDefDataTable() const
{
	static const UAxeItemSettings* AxeItemSettings = GetDefault<UAxeItemSettings>();
	UDataTable* DataTable = AxeItemSettings->ItemDefDataTable.Get();
	if (!IsValid(DataTable))
	{
		return nullptr;
	}
	return DataTable;
}

void UAxeItemSubsystem::FindItemDefRowDefine(FName RowName, FAxeItemTableRowDefine& OutItemDef)
{
	UDataTable* DataTable = GetItemDefDataTable();
	if (!IsValid(DataTable))
	{
		return;
	}
	FAxeItemTableRowDefine* Row = DataTable->FindRow<FAxeItemTableRowDefine>(RowName, FString("ItemDef"), true);
	OutItemDef = *Row;
}
