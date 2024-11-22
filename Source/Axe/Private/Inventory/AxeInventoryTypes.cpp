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
		// TODO
		// BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
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
	}
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

UItemInstance* FAxeInventoryList::AddEntry(TSubclassOf<UItemDefinition> ItemClass, int32 StackCount)
{
	check(ItemClass != nullptr);
	UItemInstance* ItemInstance = NewObject<UItemInstance>(OwnerComponent->GetOwner());
	ItemInstance->SetItemDef(ItemClass);

	AddEntry(ItemInstance, StackCount);

	return ItemInstance;
}


void FAxeInventoryList::AddEntry(UItemInstance* Instance, int32 StackCount)
{
	check(Instance);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());
	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = Instance;
	UClass* ItemDefClass = Instance->GetItemDef();
	for (UItemFragment* Fragment : GetDefault<UItemDefinition>(ItemDefClass)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;

	MarkItemDirty(NewEntry);
}

void FAxeInventoryList::RemoveEntry(UItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
