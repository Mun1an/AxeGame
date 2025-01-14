// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/InventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Engine/ActorChannel.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/EquipmentItemInstance.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Interface/ItemUseInterface.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"
#include "Item/ItemFragment/ItemFragment_UI.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitInventoryEntry();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, OwnerActor);
	DOREPLIFETIME(UInventoryComponent, AvatarActor);
}

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
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

void UInventoryComponent::ReadyForReplication()
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


void UInventoryComponent::OnInventoryItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
                                                 UItemInstance* OldItemInstance, int32 OldCount)
{
	OnInventoryChangedDelegate.Broadcast(SlotIndex, NewItemInstance, NewCount, OldItemInstance, OldCount);

	//
	const FInventoryEntry& Entry = GetInventoryEntryByIndex(SlotIndex);
	if (Entry.EntryTags.HasTag(FAxeGameplayTags::Get().Inventory_Entry_Equipment))
	{
		OnEquipmentItemChanged(SlotIndex, NewItemInstance, OldItemInstance, Entry.EntryTags);
	}
}

void UInventoryComponent::OnEquipmentItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance,
                                                 UItemInstance* OldItemInstance, FGameplayTagContainer SlotTags)
{
	OnEquipmentItemChangedDelegate.Broadcast(SlotIndex, NewItemInstance, OldItemInstance, SlotTags);

	//
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	// TODO 处理下客户端加载
	if (AxeCharacterOwner == nullptr)
	{
		return;
	}
	UAbilitySystemComponent* ASC = AxeCharacterOwner->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

	//
	UEquipmentItemInstance* NewEquipmentInstance = Cast<UEquipmentItemInstance>(NewItemInstance);
	UEquipmentItemInstance* OldEquipmentInstance = Cast<UEquipmentItemInstance>(OldItemInstance);
	if (OldEquipmentInstance)
	{
		OldEquipmentInstance->OnUnequipped();
	}
	if (NewEquipmentInstance)
	{
		NewEquipmentInstance->OnEquipped();
	}
}

bool UInventoryComponent::SetOwnerActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
	return true;
}

bool UInventoryComponent::SetAvatarActor(AActor* InAvatarActor)
{
	AvatarActor = InAvatarActor;
	return true;
}

AAxeCharacterBase* UInventoryComponent::GetAxeCharacterOwner() const
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

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::InitInventoryEntry()
{
	// init slot
	if (GetOwnerRole() == ROLE_Authority)
	{
		FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

		// inv
		for (int i = 0; i < InventoryEntryInitSize; ++i)
		{
			AddInventoryEntry();
		}

		// UseBar
		const FGameplayTagContainer UseBar(AxeGameplayTags.Inventory_Entry_UseBar);
		for (int i = 0; i < UseBarEntryInitSize; ++i)
		{
			InventoryList.AddEntry(UseBar);
		}

		// Equipment Armor
		const FGameplayTagContainer Armor_Helmet(AxeGameplayTags.Inventory_Entry_Equipment_Armor_Helmet);
		InventoryList.AddEntry(Armor_Helmet);
		const FGameplayTagContainer Armor_Chestplate(AxeGameplayTags.Inventory_Entry_Equipment_Armor_Chestplate);
		InventoryList.AddEntry(Armor_Chestplate);
		const FGameplayTagContainer Armor_Leggings(AxeGameplayTags.Inventory_Entry_Equipment_Armor_Leggings);
		InventoryList.AddEntry(Armor_Leggings);
		const FGameplayTagContainer Armor_Boots(AxeGameplayTags.Inventory_Entry_Equipment_Armor_Boots);
		InventoryList.AddEntry(Armor_Boots);

		// Equipment Weapon
		const FGameplayTagContainer Weapon(AxeGameplayTags.Inventory_Entry_Equipment_Weapon);
		InventoryList.AddEntry(Weapon);
	}
}

int32 UInventoryComponent::GetInventoryEntriesSize() const
{
	return InventoryList.Entries.Num();
}

TArray<UItemInstance*> UInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UItemInstance* UInventoryComponent::GetItemInstanceByIndex(int32 Index) const
{
	return InventoryList.GetItemInstanceByIndex(Index);
}

FInventoryEntry& UInventoryComponent::GetInventoryEntryByIndex(int32 Index)
{
	return InventoryList.GetInventoryEntryByIndex(Index);
}

void UInventoryComponent::AddItemInstance(UItemInstance* ItemInstance, int32 StackCount, int32 Index)
{
	InventoryList.AddItem(ItemInstance, StackCount, Index);
	ItemInstance->SetPawn(GetAxeCharacterOwner());

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}

	SendItemUIMessage(ItemInstance->GetItemDef(), StackCount);
}

bool UInventoryComponent::RemoveItemByIndex(int32 Index, int32 RemoveCount)
{
	return InventoryList.RemoveItem(Index, RemoveCount);
}

bool UInventoryComponent::SwapItemBySlots(int32 FromSlot, int32 ToSlot)
{
	if (FromSlot == ToSlot)
	{
		return false;
	}
	return InventoryList.SwapItem(FromSlot, ToSlot);
}

void UInventoryComponent::ServerSwapItemBySlots_Implementation(int32 FromSlot, int32 ToSlot)
{
	SwapItemBySlots(FromSlot, ToSlot);
}

void UInventoryComponent::AddInventoryEntry()
{
	FGameplayTagContainer EntryTags;
	EntryTags.AddTag(FAxeGameplayTags::Get().Inventory_Entry_Bag);
	InventoryList.AddEntry(EntryTags);
}

void UInventoryComponent::SendItemUIMessage(TSubclassOf<UItemDefinition> ItemDef, int32 StackCount)
{
	const UItemFragment* Fragment = UItemFunctionLibrary::FindItemDefinitionFragment(
		ItemDef, UItemFragment_UI::StaticClass());
	const UItemFragment_UI* ItemFragment_UI = Cast<UItemFragment_UI>(Fragment);

	UTexture2D* Texture2D = ItemFragment_UI->Icon;
	FText DisplayName = ItemFragment_UI->DisplayName;

	// OnSendInventoryItemUIMessage.Broadcast(Texture2D, DisplayName, StackCount);
	ClientSendItemUIMessage(Texture2D, DisplayName, StackCount);
}

void UInventoryComponent::GetEquipmentEntryArray(TArray<FInventoryEntry>& OutEntries) const
{
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		if (Entry.EntryTags.HasTag(AxeGameplayTags.Inventory_Entry_Equipment))
		{
			OutEntries.Add(Entry);
		}
	}
}

void UInventoryComponent::GetUseItemEntryArray(TArray<FInventoryEntry>& OutEntries) const
{
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	for (const FInventoryEntry& Entry : InventoryList.Entries)
	{
		if (Entry.EntryTags.HasTag(AxeGameplayTags.Inventory_Entry_UseBar))
		{
			OutEntries.Add(Entry);
		}
	}
}

void UInventoryComponent::RefreshInventoryItemEntryChange()
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

bool UInventoryComponent::CheckEntryHasTag(const FGameplayTag CheckTag, FInventoryEntry& Entry)
{
	return Entry.EntryTags.HasTag(CheckTag);
}

bool UInventoryComponent::CheckEntryCanItemUse(const int32 SlotIndex)
{
	const FInventoryEntry Entry = GetInventoryEntryByIndex(SlotIndex);
	if (!Entry.EntryTags.HasTag(FAxeGameplayTags::Get().Inventory_Entry_UseBar))
	{
		return false;
	}
	if (Entry.StackCount <= 0 || Entry.Instance == nullptr)
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


void UInventoryComponent::ClientSendItemUIMessage_Implementation(UTexture2D* Texture, const FText& ItemName,
                                                                 int32 StackCount)
{
	OnSendInventoryItemUIMessage.Broadcast(Texture, ItemName, StackCount);
}
