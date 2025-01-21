// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/InventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Engine/ActorChannel.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Inventory/Component/ShopComponent.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/EquipmentItemInstance.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Interface/ItemUseInterface.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"
#include "Item/ItemFragment/ItemFragment_UI.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::OnInventoryItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
	UItemInstance* OldItemInstance, int32 OldCount)
{
	Super::OnInventoryItemChanged(SlotIndex, NewItemInstance, NewCount, OldItemInstance, OldCount);
	
	//
	const FInventoryEntry& Entry = GetInventoryEntryByIndex(SlotIndex);
	if (Entry.EntryTags.HasTag(FAxeGameplayTags::Get().Inventory_Entry_Equipment))
	{
		OnEquipmentItemChanged(SlotIndex, NewItemInstance, OldItemInstance, Entry.EntryTags);
	}
}

void UInventoryComponent::InitInventoryEntry()
{
	Super::InitInventoryEntry();
	// init slot
	if (GetOwnerRole() == ROLE_Authority)
	{
		FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

		// // inv
		// for (int i = 0; i < InventoryEntryInitSize; ++i)
		// {
		// 	AddInventoryEntry();
		// }

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

void UInventoryComponent::ServerBuyShopItem_Implementation(UShopComponent* ShopComponent, int32 ShopSlot)
{
	ShopComponent->BuyShopItem(ShopSlot, this);
}

