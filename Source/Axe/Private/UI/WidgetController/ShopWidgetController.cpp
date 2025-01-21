// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ShopWidgetController.h"

#include "Inventory/Component/InventoryComponent.h"
#include "Inventory/Component/ShopComponent.h"
#include "PlayerState/AxePlayerState.h"

UShopWidgetController::UShopWidgetController()
{
	InventoryBoxOptionSelections = {
		EInventoryBoxOptionSelection::Drop,
		EInventoryBoxOptionSelection::Sell,
	};
}

void UShopWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	Super::SetWidgetControllerParams(Params);
}

void UShopWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const AAxePlayerState* AxePS = GetAxePlayerState();

	OnGoldCoinCountChanged(AxePS->GetGoldCoinCount(), 0);
}

void UShopWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	// bind
	ShopComponent->OnInventoryChangedDelegate.AddDynamic(
		this, &UShopWidgetController::OnShopChangedCallback);

	AAxePlayerState* AxePS = GetAxePlayerState();
	AxePS->OnGoldCoinCountChangedDelegate.AddUObject(this, &UShopWidgetController::OnGoldCoinCountChanged);
}

void UShopWidgetController::TryBuyShopItem(int32 ShopSlot)
{
	bool bCheckCanBuyShopItemClient = ShopComponent->CheckCanBuyShopItemByClient(ShopSlot, InventoryComponent);
	if (bCheckCanBuyShopItemClient)
	{
		InventoryComponent->ServerBuyShopItem(ShopComponent, ShopSlot);
	}
}

void UShopWidgetController::TryRefreshShopItem()
{
	bool bCheck = ShopComponent->CheckCanRefreshShopItemByClient(InventoryComponent);
	if (bCheck)
	{
		InventoryComponent->ServerRefreshShopItem(ShopComponent);
	}
}

void UShopWidgetController::OnShopChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
                                                  UItemInstance* OldItemInstance, int32 OldCount)
{
	OnShopChangedInWidgetController.Broadcast(SlotIndex, NewItemInstance, NewCount, OldCount);
}

void UShopWidgetController::OnGoldCoinCountChanged(int32 NewValue, int32 OldValue)
{
	OnGoldCoinCountChangedDelegate.Broadcast(NewValue);
}
