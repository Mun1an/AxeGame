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
	bool GetInventoryEntryByIndex(int32 Index, FInventoryEntry& InventoryEntry);

	TArray<UItemInstance*> GetAllItems() const;
	UItemInstance* GetItemInstanceByIndex(int32 Index) const;

	void AddEntry();
	//
	void AddItem(UItemInstance* ItemInstance, int32 StackCount = 1, int32 SlotIndex = INDEX_NONE);

	bool RemoveItemByIndex(int32 Index, int32 RemoveCount = 1);
	
	int32 GetEmptyOrStackSlotIndex(UItemInstance* ItemInstance);
};

template <>
struct TStructOpsTypeTraits<FAxeInventoryList> : public TStructOpsTypeTraitsBase2<FAxeInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
