// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/EntryBaseComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Engine/ActorChannel.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Interface/ItemUseInterface.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"
#include "Item/ItemFragment/ItemFragment_UI.h"
#include "Net/UnrealNetwork.h"

UEntryBaseComponent::UEntryBaseComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UEntryBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	InitInventoryEntry();
}

void UEntryBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEntryBaseComponent, InventoryList);
	DOREPLIFETIME(UEntryBaseComponent, OwnerActor);
	DOREPLIFETIME(UEntryBaseComponent, AvatarActor);
}

bool UEntryBaseComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                              FReplicationFlags* RepFlags)
{
	bool WroteSomething = false;
	WroteSomething |= Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UEntryBaseComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryEntry& Entry : InventoryList.Entries)
		{
			UItemInstance* Instance = Entry.Instance;
			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}


void UEntryBaseComponent::OnInventoryItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
                                                 UItemInstance* OldItemInstance, int32 OldCount)
{
	OnInventoryChangedDelegate.Broadcast(SlotIndex, NewItemInstance, NewCount, OldItemInstance, OldCount);
}


bool UEntryBaseComponent::SetOwnerActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
	return true;
}

bool UEntryBaseComponent::SetAvatarActor(AActor* InAvatarActor)
{
	AvatarActor = InAvatarActor;
	return true;
}

AAxeCharacterBase* UEntryBaseComponent::GetAxeCharacterOwner() const
{
	if (AActor* LocalAvatarActor = GetAvatarActor())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = GetOwner();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

void UEntryBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEntryBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UEntryBaseComponent::InitInventoryEntry()
{
	// init slot
	if (GetOwnerRole() == ROLE_Authority)
	{
		// inv
		for (int i = 0; i < InventoryEntryInitSize; ++i)
		{
			AddInventoryEntry();
		}
	}
}

int32 UEntryBaseComponent::GetInventoryEntriesSize() const
{
	return InventoryList.Entries.Num();
}

TArray<UItemInstance*> UEntryBaseComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

void UEntryBaseComponent::GetAllEntryArray(TArray<FInventoryEntry>& OutEntries) const
{
	OutEntries = InventoryList.Entries;
}

UItemInstance* UEntryBaseComponent::GetItemInstanceByIndex(int32 Index) const
{
	return InventoryList.GetItemInstanceByIndex(Index);
}

FInventoryEntry& UEntryBaseComponent::GetInventoryEntryByIndex(int32 Index)
{
	return InventoryList.GetInventoryEntryByIndex(Index);
}

bool UEntryBaseComponent::AddItemInstance(UItemInstance* ItemInstance, int32 StackCount, int32 Index)
{
	bool bResult = InventoryList.AddItem(ItemInstance, StackCount, Index, FAxeGameplayTags::Get().Inventory_Entry_Bag);
	if (!bResult)
	{
		return false;
	}
	ItemInstance->SetPawn(GetAxeCharacterOwner());

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}

	SendItemUIMessage(ItemInstance->GetItemDef(), StackCount);
	return true;
}

bool UEntryBaseComponent::RemoveItemByIndex(int32 Index, int32 RemoveCount)
{
	return InventoryList.RemoveItem(Index, RemoveCount);
}

bool UEntryBaseComponent::SwapItemBySlots(int32 FromSlot, int32 ToSlot)
{
	if (FromSlot == ToSlot)
	{
		return false;
	}
	return InventoryList.SwapItem(FromSlot, ToSlot);
}

void UEntryBaseComponent::DropItemBySlot(int32 Slot)
{
	if (!CheckEntryHasItem(Slot))
	{
		return;
	}
	int32 DropCount = 1;
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	UItemInstance* ItemInstanceByIndex = GetItemInstanceByIndex(Slot);
	AAxeWorldItemActor* AxeWorldItemActor = UItemFunctionLibrary::CreateWorldItemActor(
		AxeCharacterOwner,
		AxeCharacterOwner->GetActorTransform(),
		ItemInstanceByIndex,
		DropCount
	);
	if (AxeWorldItemActor)
	{
		RemoveItemByIndex(Slot, DropCount);
	}
}


void UEntryBaseComponent::ServerSwapItemBySlots_Implementation(int32 FromSlot, int32 ToSlot)
{
	SwapItemBySlots(FromSlot, ToSlot);
}

void UEntryBaseComponent::ServerRemoveItemBySlot_Implementation(int32 Slot)
{
	RemoveItemByIndex(Slot);
}

void UEntryBaseComponent::ServerDropItemBySlot_Implementation(int32 Slot)
{
	DropItemBySlot(Slot);
}

void UEntryBaseComponent::AddInventoryEntry()
{
	FGameplayTagContainer EntryTags;
	EntryTags.AddTag(FAxeGameplayTags::Get().Inventory_Entry_Bag);
	InventoryList.AddEntry(EntryTags);
}

void UEntryBaseComponent::SendItemUIMessage(TSubclassOf<UItemDefinition> ItemDef, int32 StackCount)
{
	const UItemFragment* Fragment = UItemFunctionLibrary::FindItemDefinitionFragment(
		ItemDef, UItemFragment_UI::StaticClass());
	const UItemFragment_UI* ItemFragment_UI = Cast<UItemFragment_UI>(Fragment);

	UTexture2D* Texture2D = ItemFragment_UI->Icon;
	FText DisplayName = ItemFragment_UI->DisplayName;

	// OnSendInventoryItemUIMessage.Broadcast(Texture2D, DisplayName, StackCount);
	ClientSendItemUIMessage(Texture2D, DisplayName, StackCount);
}

void UEntryBaseComponent::RefreshInventoryItemEntryChange()
{
	// 调用触发所有的 Inventory Entry Item Change
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		OnInventoryItemChanged(
			Entry.SlotIndex, Entry.Instance, Entry.StackCount,
			Entry.LastInstance, Entry.LastObservedCount
		);
	}
}

bool UEntryBaseComponent::CheckEntryHasTag(const FGameplayTag CheckTag, FInventoryEntry& Entry)
{
	return Entry.EntryTags.HasTag(CheckTag);
}

bool UEntryBaseComponent::CheckEntryHasItem(int32 SlotIndex)
{
	const FInventoryEntry Entry = GetInventoryEntryByIndex(SlotIndex);
	return Entry.StackCount > 0 && Entry.Instance != nullptr;
}

bool UEntryBaseComponent::CheckEntryCanItemUse(const int32 SlotIndex)
{
	if (!CheckEntryHasItem(SlotIndex))
	{
		return false;
	}
	const FInventoryEntry Entry = GetInventoryEntryByIndex(SlotIndex);
	if (!Entry.EntryTags.HasTag(FAxeGameplayTags::Get().Inventory_Entry_UseBar))
	{
		return false;
	}
	if (IItemUseInterface* ItemUseInterface = Cast<IItemUseInterface>(Entry.Instance))
	{
		if (!ItemUseInterface->CanUseItem())
		{
			return false;
		}
	}
	return true;
}

bool UEntryBaseComponent::CheckCanAddItemInstance(UItemInstance* ItemInstance, int32 ItemCount, FGameplayTag SlotTag)
{
	TMap<int32, int32> SlotCountMap;
	return InventoryList.GetStackOrEmptySlotIndex(ItemInstance, SlotCountMap, ItemCount, SlotTag);
}

void UEntryBaseComponent::ClientSendItemUIMessage_Implementation(UTexture2D* Texture, const FText& ItemName,
                                                                 int32 StackCount)
{
	OnSendInventoryItemUIMessage.Broadcast(Texture, ItemName, StackCount);
}
