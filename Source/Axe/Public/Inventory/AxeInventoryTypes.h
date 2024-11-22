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

	UPROPERTY()
	TObjectPtr<UItemInstance> Instance = nullptr;

	UPROPERTY()
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

	TArray<UItemInstance*> GetAllItems() const;
	UItemInstance* GetItemInstanceByIndex(int32 Index) const;
	UItemInstance* AddEntry(TSubclassOf<UItemDefinition> ItemClass, int32 StackCount = 1);
	void AddEntry(UItemInstance* Instance, int32 StackCount = 1);
	void RemoveEntry(UItemInstance* Instance);
};

template <>
struct TStructOpsTypeTraits<FAxeInventoryList> : public TStructOpsTypeTraitsBase2<FAxeInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
