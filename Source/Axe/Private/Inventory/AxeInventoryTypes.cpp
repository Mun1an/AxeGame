#include "Inventory/AxeInventoryTypes.h"

#include "Inventory/Component/InventoryComponent.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"

void FAxeInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		// BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;

		OwnerComponent->OnInventoryItemChanged(Index, Stack.Instance, Stack.StackCount, Stack.LastObservedCount);
	}
}

void FAxeInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];

		// BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FAxeInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		// BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;

		OwnerComponent->OnInventoryItemChanged(Index, Stack.Instance, Stack.StackCount, Stack.LastObservedCount);
	}
}

FInventoryEntry& FAxeInventoryList::GetInventoryEntryByIndex(int32 Index)
{
	return Entries[Index];
}

TArray<UItemInstance*> FAxeInventoryList::GetAllItems() const
{
	TArray<UItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FInventoryEntry& Entry : Entries)
	{
		Results.Add(Entry.Instance);
	}
	return Results;
}

UItemInstance* FAxeInventoryList::GetItemInstanceByIndex(int32 Index) const
{
	if (Index >= Entries.Num())
	{
		return nullptr;
	}
	return Entries[Index].Instance;
}

void FAxeInventoryList::AddEntry(const FGameplayTagContainer& EntryTags)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = nullptr;
	NewEntry.StackCount = 0;
	NewEntry.SlotIndex = NewEntryIndex;
	NewEntry.EntryTags = EntryTags;

	NewEntryIndex++;

	HandleEntryChanged(NewEntry);
}

bool FAxeInventoryList::AddItem(UItemInstance* ItemInstance, int32 StackCount, int32 SlotIndex)
{
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	TMap<int32, int32> SlotCountMap;
	if (SlotIndex == INDEX_NONE)
	{
		bool bGet = GetStackOrEmptySlotIndex(ItemInstance, SlotCountMap, StackCount);
		if (!bGet)
		{
			return false;
		}
	}
	else
	{
		SlotCountMap.Add(SlotIndex, StackCount);
	}

	bool bResult = true;
	for (TTuple<int, int> CountMap : SlotCountMap)
	{
		const int32 InSlot = CountMap.Key;
		const int32 InCount = CountMap.Value;

		bResult = bResult & AddItemInternal(ItemInstance, InCount, InSlot);
	}
	return bResult;
}

bool FAxeInventoryList::AddItemInternal(UItemInstance* ItemInstance, int32 StackCount, int32 SlotIndex)
{
	FInventoryEntry& Entry = Entries[SlotIndex];

	bool bCanPutInEntry = CheckCanPutInEntry(Entry, ItemInstance);
	if (!bCanPutInEntry)
	{
		return false;
	}
	
	if (Entry.Instance && Entry.Instance->GetItemDef() == ItemInstance->GetItemDef())
	{
		return ChangeItemStackCount(Entry, StackCount + Entry.StackCount);
	}

	Entry.Instance = ItemInstance;
	Entry.StackCount = StackCount;

	HandleEntryChanged(Entry);
	return true;
}

bool FAxeInventoryList::RemoveItem(int32 SlotIndex, int32 RemoveCount)
{
	FInventoryEntry& Entry = Entries[SlotIndex];

	if (Entry.Instance == nullptr)
	{
		return false;
	}
	Entry.Instance = nullptr;
	Entry.StackCount = FMath::Max(0, Entry.StackCount - RemoveCount);

	HandleEntryChanged(Entry);
	return true;
}

bool FAxeInventoryList::SwapItem(int32 FromSlot, int32 ToSlot)
{
	FInventoryEntry& FromEntry = Entries[FromSlot];
	FInventoryEntry& ToEntry = Entries[ToSlot];

	if (FromEntry.Instance == nullptr && ToEntry.Instance == nullptr)
	{
		return false;
	}

	const bool bCanPutInEntry = CheckCanPutInEntry(FromEntry, ToEntry.Instance);
	const bool bCanPutInEntry_1 = CheckCanPutInEntry(ToEntry, FromEntry.Instance);
	if (!bCanPutInEntry || !bCanPutInEntry_1)
	{
		return false;
	}

	// 判断堆叠
	if (FromEntry.Instance && ToEntry.Instance && FromEntry.Instance->GetItemDef() == ToEntry.Instance->GetItemDef()
	)
	{
		const UItemDefinition* ItemDef = GetDefault<UItemDefinition>(FromEntry.Instance->GetItemDef());
		const int32 ItemMaxStackSize = ItemDef->GetItemMaxStackSize();
		int32 TotalCount = ToEntry.StackCount + FromEntry.StackCount;

		if (TotalCount <= ItemMaxStackSize)
		{
			ToEntry.StackCount = TotalCount;
			FromEntry.StackCount = 0;
		}
		else if (ToEntry.StackCount < ItemMaxStackSize)
		{
			ToEntry.StackCount = ItemMaxStackSize;
			FromEntry.StackCount = TotalCount - ItemMaxStackSize;
		}
		else
		{
			int32 TempStackCount = FromEntry.StackCount;
			FromEntry.StackCount = ToEntry.StackCount;
			ToEntry.StackCount = TempStackCount;
		}
	}
	else
	{
		TObjectPtr<UItemInstance> TempItemInstance = FromEntry.Instance;
		int32 TempStackCount = FromEntry.StackCount;

		FromEntry.Instance = ToEntry.Instance;
		FromEntry.StackCount = ToEntry.StackCount;
		ToEntry.Instance = TempItemInstance;
		ToEntry.StackCount = TempStackCount;
	}

	HandleEntryChanged(FromEntry);
	HandleEntryChanged(ToEntry);
	return true;
}

bool FAxeInventoryList::ChangeItemStackCount(FInventoryEntry& Entry, int32 NewCount)
{
	Entry.StackCount = NewCount;

	HandleEntryChanged(Entry);
	return true;
}

bool FAxeInventoryList::GetStackOrEmptySlotIndex(UItemInstance* ItemInstance, TMap<int32, int32>& SlotCountMap,
                                                 int32 NeedCount)
{
	const UItemDefinition* ItemDef = GetDefault<UItemDefinition>(ItemInstance->GetItemDef());
	const int32 ItemMaxStackSize = ItemDef->GetItemMaxStackSize();
	//
	int32 RemainingNeedCount = NeedCount;

	for (int32 Index = 0; Index < Entries.Num(); ++Index)
	{
		FInventoryEntry& Entry = Entries[Index];
		if (Entry.Instance == nullptr)
		{
			continue;
		}
		if (Entry.Instance->GetItemDef() == ItemInstance->GetItemDef() && Entry.StackCount < ItemMaxStackSize)
		{
			//
			int32 CanAddCount = ItemMaxStackSize - Entry.StackCount;
			if (CanAddCount > 0)
			{
				if (RemainingNeedCount < CanAddCount)
				{
					RemainingNeedCount = 0;
					SlotCountMap.Add(Index, RemainingNeedCount);
					break;
				}
				else
				{
					RemainingNeedCount -= CanAddCount;
					SlotCountMap.Add(Index, CanAddCount);
				}
			}
		}
	}
	//
	if (RemainingNeedCount > 0)
	{
		for (int32 Index = 0; Index < Entries.Num(); ++Index)
		{
			if (Entries[Index].Instance == nullptr)
			{
				if (RemainingNeedCount > ItemMaxStackSize)
				{
					RemainingNeedCount -= ItemMaxStackSize;
					SlotCountMap.Add(Index, ItemMaxStackSize);
				}
				else
				{
					SlotCountMap.Add(Index, RemainingNeedCount);
					RemainingNeedCount = 0;
					break;
				}
			}
		}
	}

	if (RemainingNeedCount > 0)
	{
		// 如果还是装不下
		// FIXME
		return false;
	}

	return true;
}

void FAxeInventoryList::HandleEntryChanged(FInventoryEntry& Entry)
{
	// 改变Entry时手动调用
	if (Entry.SlotIndex == INDEX_NONE)
	{
		return;
	}
	if (Entry.StackCount <= 0)
	{
		Entry.Instance = nullptr;
	}

	OwnerComponent->OnInventoryItemChanged(
		Entry.SlotIndex, Entry.Instance, Entry.StackCount, Entry.LastObservedCount
	);
	MarkItemDirty(Entry);
}

bool FAxeInventoryList::CheckCanPutInEntry(FInventoryEntry& Entry, UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		return true;
	}
	const UItemDefinition* ItemDef = GetDefault<UItemDefinition>(ItemInstance->GetItemDef());
	const UItemFragment_CommonInfo* Fragment = ItemDef->GetCommonInfoFragment();
	if (!Fragment)
	{
		return false;
	}
	const bool bHasAny = Fragment->EntryTags.HasAny(Entry.EntryTags);
	return bHasAny;
}
