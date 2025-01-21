// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/ShopComponent.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Inventory/ShopItemDataAsset.h"
#include "Item/ItemFunctionLibrary.h"
#include "PlayerState/AxePlayerState.h"
#include "TipsMessage/TipsMessageFunctionLibrary.h"

UShopComponent::UShopComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	InventoryEntryInitSize = 4;
}

bool UShopComponent::RandomSpawnShopItem()
{
	if (!ShopItemDataAsset)
	{
		return false;
	}
	TArray<FShopItemDataInfo> ShopItemDataInfos = ShopItemDataAsset->ShopItemDataInfos;
	if (ShopItemDataInfos.Num() <= 0)
	{
		return false;
	}

	for (int i = 0; i < InventoryEntryInitSize; ++i)
	{
		const FInventoryEntry& InventoryEntryByIndex = GetInventoryEntryByIndex(i);
		RemoveItemByIndex(i, InventoryEntryByIndex.StackCount);
	}

	for (int i = 0; i < InventoryEntryInitSize; ++i)
	{
		const int32 RandRange = FMath::RandRange(0, ShopItemDataInfos.Num() - 1);
		const FShopItemDataInfo ShopItemDataInfo = ShopItemDataInfos[RandRange];
		UItemInstance* ShopItemInstance = CreateShopItemInstance(ShopItemDataInfo);
		if (!ShopItemInstance)
		{
			continue;
		}
		const int32 RandomItemCount = FMath::RandRange(
			ShopItemDataInfo.MinItemCount, ShopItemDataInfo.MaxItemCount
		);
		AddItemInstance(
			ShopItemInstance, RandomItemCount, i
		);
	}
	return true;
}

UItemInstance* UShopComponent::CreateShopItemInstance(const FShopItemDataInfo& ShopItemDataInfo)
{
	UWorld* World = GetWorld();
	UItemInstance* ItemInstance = UItemFunctionLibrary::CreateItemInstance(World, ShopItemDataInfo.ItemDef);
	float ItemPriceRate = FMath::RandRange(
		ShopItemDataInfo.MinDefaultPriceRate, ShopItemDataInfo.MaxDefaultPriceRate
	);
	int32 ShopItemPrice = GetShopItemPrice(ItemInstance, ItemPriceRate);
	ItemInstance->SetItemInstancePrice(ShopItemPrice);
	//
	ItemInstance->FinishItemInstanceCreated();

	return ItemInstance;
}

int32 UShopComponent::GetShopItemPrice(UItemInstance* ItemInstance, float PriceRate)
{
	int32 ItemInstanceDefaultPrice = ItemInstance->GetItemInstanceDefaultPrice();
	return ItemInstanceDefaultPrice * PriceRate;
}

int32 UShopComponent::GetShopItemTotalPriceBySlot(int32 Slot)
{
	const UItemInstance* ItemInstance = GetItemInstanceByIndex(Slot);
	if (!ItemInstance)
	{
		return 0;
	}
	const int32 ItemInstancePrice = ItemInstance->GetItemInstancePrice();
	const int32 StackCount = GetInventoryEntryByIndex(Slot).StackCount;
	const int32 TotalPrice = ItemInstancePrice * StackCount;
	return TotalPrice;
}

bool UShopComponent::BuyShopItem(int32 ShopSlot, UEntryBaseComponent* BuyerInventory)
{
	const FInventoryEntry& ShopEntry = GetInventoryEntryByIndex(ShopSlot);
	UItemInstance* ShopItemInstance = ShopEntry.Instance;
	if (!ShopItemInstance)
	{
		return false;
	}
	const int32 ShopTotalItemPrice = GetShopItemTotalPriceBySlot(ShopSlot);
	AAxeCharacterBase* BuyerAxeCharacter = BuyerInventory->GetAxeCharacterOwner();
	APlayerState* PS = BuyerAxeCharacter->GetPlayerState();
	if (!PS)
	{
		return false;
	}
	AAxePlayerState* AxePS = Cast<AAxePlayerState>(PS);
	// GoldCoin
	const int32 OwnerGoldCoinCount = AxePS->GetGoldCoinCount();
	if (OwnerGoldCoinCount < ShopTotalItemPrice)
	{
		return false;
	}
	// Add Item
	ShopItemInstance->SetItemInstancePrice(ShopItemInstance->GetItemInstanceDefaultPrice());
	ShopItemInstance->UpdateItemInstance();
	bool bAddBuyerItem = BuyerInventory->AddItemInstance(ShopItemInstance, ShopEntry.StackCount);
	if (!bAddBuyerItem)
	{
		return false;
	}
	bool bRemoveItemByIndex = RemoveItemByIndex(ShopSlot, ShopEntry.StackCount);
	UAbilitySystemComponent* ASC = AxePS->GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	AxeASC->ApplyIncomingGoldCoinCountEffect(BuyerAxeCharacter, -ShopTotalItemPrice);

	return bRemoveItemByIndex;
}

bool UShopComponent::CheckCanBuyShopItemByClient(int32 ShopSlot, UEntryBaseComponent* BuyerInventory)
{
	const FInventoryEntry& ShopEntry = GetInventoryEntryByIndex(ShopSlot);
	UItemInstance* ShopItemInstance = ShopEntry.Instance;
	if (!ShopItemInstance)
	{
		return false;
	}
	const int32 ShopTotalItemPrice = GetShopItemTotalPriceBySlot(ShopSlot);
	AAxeCharacterBase* BuyerAxeCharacter = BuyerInventory->GetAxeCharacterOwner();
	APlayerState* PS = BuyerAxeCharacter->GetPlayerState();
	if (!PS)
	{
		return false;
	}
	AAxePlayerState* AxePS = Cast<AAxePlayerState>(PS);
	// GoldCoin
	const int32 OwnerGoldCoinCount = AxePS->GetGoldCoinCount();
	if (OwnerGoldCoinCount < ShopTotalItemPrice)
	{
		UTipsMessageFunctionLibrary::SendTipsMessage(BuyerAxeCharacter, TEXT("金币不足"), 3.0f);
		return false;
	}
	// Add Item
	ShopItemInstance->SetItemInstancePrice(ShopItemInstance->GetItemInstanceDefaultPrice());
	ShopItemInstance->UpdateItemInstance();
	bool bCanAddItemInstance = BuyerInventory->CheckCanAddItemInstance(ShopItemInstance, ShopEntry.StackCount,
	                                                                   FAxeGameplayTags::Get().Inventory_Entry_Bag);
	if (!bCanAddItemInstance)
	{
		UTipsMessageFunctionLibrary::SendTipsMessage(BuyerAxeCharacter, TEXT("背包已满"), 3.0f);
		return false;
	}
	return true;
}

bool UShopComponent::RefreshShopItem(UEntryBaseComponent* BuyerInventory)
{
	AAxeCharacterBase* BuyerAxeCharacter = BuyerInventory->GetAxeCharacterOwner();
	APlayerState* PS = BuyerAxeCharacter->GetPlayerState();
	if (!PS)
	{
		return false;
	}
	AAxePlayerState* AxePS = Cast<AAxePlayerState>(PS);
	// GoldCoin
	const int32 OwnerGoldCoinCount = AxePS->GetGoldCoinCount();
	if (OwnerGoldCoinCount < RefreshShopItemCost)
	{
		return false;
	}
	UAbilitySystemComponent* ASC = AxePS->GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	AxeASC->ApplyIncomingGoldCoinCountEffect(BuyerAxeCharacter, -RefreshShopItemCost);

	bool bRandomSpawnShopItem = RandomSpawnShopItem();
	return bRandomSpawnShopItem;
}

bool UShopComponent::CheckCanRefreshShopItemByClient(UEntryBaseComponent* BuyerInventory)
{
	AAxeCharacterBase* BuyerAxeCharacter = BuyerInventory->GetAxeCharacterOwner();
	APlayerState* PS = BuyerAxeCharacter->GetPlayerState();
	if (!PS)
	{
		return false;
	}
	AAxePlayerState* AxePS = Cast<AAxePlayerState>(PS);
	// GoldCoin
	const int32 OwnerGoldCoinCount = AxePS->GetGoldCoinCount();
	if (OwnerGoldCoinCount < RefreshShopItemCost)
	{
		UTipsMessageFunctionLibrary::SendTipsMessage(BuyerAxeCharacter, TEXT("金币不足"), 3.0f);
		return false;
	}
	return true;
}

void UShopComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		RandomSpawnShopItem();
	}
}
