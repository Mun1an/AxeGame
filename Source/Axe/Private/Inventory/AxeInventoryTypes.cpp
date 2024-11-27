#include "Inventory/AxeInventoryTypes.h"

#include "Inventory/Component/InventoryComponent.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"

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

bool FAxeInventoryList::GetInventoryEntryByIndex(int32 Index, FInventoryEntry& InventoryEntry)
{
	if (Index >= Entries.Num())
	{
		return false;
	}
	InventoryEntry = Entries[Index];
	return true;
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

void FAxeInventoryList::AddEntry()
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = nullptr;
	NewEntry.StackCount = 0;

	MarkItemDirty(NewEntry);
}

void FAxeInventoryList::AddItem(UItemInstance* ItemInstance, int32 StackCount, int32 SlotIndex)
{
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	if (SlotIndex == INDEX_NONE)
	{
		SlotIndex = GetEmptyOrStackSlotIndex(ItemInstance);
	}
	if (SlotIndex == INDEX_NONE)
	{
		return;
	}
	FInventoryEntry& Entry = Entries[SlotIndex];
	Entry.Instance = ItemInstance;
	Entry.StackCount = StackCount;

	OwnerComponent->OnInventoryItemChanged(SlotIndex, ItemInstance, StackCount, Entry.LastObservedCount);

	MarkItemDirty(Entry);
}

bool FAxeInventoryList::RemoveItemByIndex(int32 Index, int32 RemoveCount)
{
	if (Index >= Entries.Num())
	{
		return false;
	}
	FInventoryEntry& Entry = Entries[Index];
	if (Entry.Instance == nullptr)
	{
		return false;
	}
	Entry.Instance = nullptr;
	Entry.StackCount = FMath::Max(0, Entry.StackCount - RemoveCount);

	OwnerComponent->OnInventoryItemChanged(
		Index, Entry.Instance, Entry.StackCount, Entry.LastObservedCount
	);
	MarkItemDirty(Entry);
	return true;
}

int32 FAxeInventoryList::GetEmptyOrStackSlotIndex(UItemInstance* ItemInstance)
{
	// TODO 堆叠
	for (int32 Index = 0; Index < Entries.Num(); ++Index)
	{
		if (Entries[Index].Instance == nullptr)
		{
			return Index;
		}
	}
	return INDEX_NONE;
}
