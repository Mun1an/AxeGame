#pragma once
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "AxeInventoryTypes.generated.h"

struct FAxeInventoryItemSlotArray;
class UItemInstance;
class UInventoryComponent;

#define INV_FASTARRAYSERIALIZER_TARRAY_ACCESSORS(PropertyName) \
	auto begin() { return PropertyName.begin(); }	\
	auto begin() const { return PropertyName.begin(); } \
	auto end() { return PropertyName.end(); } \
	auto end() const { return PropertyName.end(); } \
	auto& operator[](int32 index) { return PropertyName[index]; } \
	const auto& operator[](int32 index) const { return PropertyName[index]; } \
	int32 Num() const { return PropertyName.Num(); }

USTRUCT(BlueprintType)
struct AXE_API FInventoryItemSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryItemSlot()
		: FFastArraySerializerItem()
		  , ItemInstance(nullptr)
		  , SlotId(INDEX_NONE)
		  , SlotTags()
		  , OldItemInstance(nullptr)
		  , ParentInventory(nullptr)
		  , MostRecentChangeContext()
	{
	}

	FInventoryItemSlot(const FInventoryItemSlot& Copy)
		: FFastArraySerializerItem(Copy)
		  , ItemInstance(Copy.ItemInstance)
		  , SlotId(Copy.SlotId)
		  , SlotTags(Copy.SlotTags)
		  , OldItemInstance(nullptr)
		  , ParentInventory(Copy.ParentInventory)
		  , MostRecentChangeContext()
	{
	}

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = Inventory)
	UItemInstance* ItemInstance;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = Inventory)
	int32 SlotId;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = Inventory)
	FGameplayTagContainer SlotTags;

	TWeakObjectPtr<UItemInstance> OldItemInstance;

	static FInventoryItemSlot Invalid;

	/** Comparison operator */
	bool operator==(FInventoryItemSlot const& Other) const
	{
		return SlotId == Other.SlotId && ItemInstance == Other.ItemInstance && MostRecentChangeContext == Other.
			MostRecentChangeContext;
	}

	/** Comparison operator */
	bool operator!=(FInventoryItemSlot const& Other) const
	{
		return !(operator==(Other));
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);

	void PreReplicatedRemove(const FAxeInventoryItemSlotArray& InArraySerializer);
	void PostReplicatedAdd(const FAxeInventoryItemSlotArray& InArraySerializer);
	void PostReplicatedChange(const FAxeInventoryItemSlotArray& InArraySerializer);

private:
	UPROPERTY()
	UInventoryComponent* ParentInventory;

	UPROPERTY()
	FGameplayTag MostRecentChangeContext;

	friend class UInventoryComponent;
};


USTRUCT()
struct FAxeInventoryItemSlotArray : public FFastArraySerializer
{
	GENERATED_BODY()

	FAxeInventoryItemSlotArray()
		: FAxeInventoryItemSlotArray(nullptr)
	{
	}

	FAxeInventoryItemSlotArray(UInventoryComponent* InOwner)
		: Owner(InOwner)
	{
	}

	UPROPERTY()
	TArray<FInventoryItemSlot> Slots;

	UPROPERTY()
	UInventoryComponent* Owner;

	// bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	// {
	// 	return FFastArraySerializer::FastArrayDeltaSerialize<FArcInventoryItemSlot, FAxeInventoryItemSlotArray>(Slots, DeltaParms, *this);
	// }
	//
	INV_FASTARRAYSERIALIZER_TARRAY_ACCESSORS(Slots)
};
