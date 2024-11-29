#pragma once
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "AxeInventoryTypes.generated.h"

class UItemDefinition;
class FInventoryList;
struct FAxeInventoryList;
class UItemInstance;
class UInventoryComponent;

USTRUCT(BlueprintType)
struct AXE_API FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry()
	{
	}

	friend FInventoryList;
	friend UInventoryComponent;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};


USTRUCT()
struct FAxeInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FAxeInventoryList()
		: FAxeInventoryList(nullptr)
	{
	}

	FAxeInventoryList(UInventoryComponent* InOwner)
		: OwnerComponent(InOwner)
	{
	}

	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	UInventoryComponent* OwnerComponent;

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FAxeInventoryList>(
			Entries, DeltaParms, *this);
	}

	//
	FInventoryEntry& GetInventoryEntryByIndex(int32 Index);

	TArray<UItemInstance*> GetAllItems() const;
	UItemInstance* GetItemInstanceByIndex(int32 Index) const;

	void AddEntry();
	//
	bool AddItem(UItemInstance* ItemInstance, int32 StackCount = 1, int32 SlotIndex = INDEX_NONE);

	bool RemoveItem(int32 SlotIndex, int32 RemoveCount = 1);

	bool SwapItem(int32 FromSlot, int32 ToSlot);

	bool ChangeItemStackCount(FInventoryEntry& Entry, int32 NewCount);

	bool GetStackOrEmptySlotIndex(UItemInstance* ItemInstance, TMap<int32, int32>& SlotCountMap, int32 NeedCount);

	void HandleEntryChanged(FInventoryEntry& Entry);

protected:
	bool AddItemInternal(UItemInstance* ItemInstance, int32 StackCount, int32 SlotIndex);

private:
	int32 NewEntryIndex = 0;
};

template <>
struct TStructOpsTypeTraits<FAxeInventoryList> : public TStructOpsTypeTraitsBase2<FAxeInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
