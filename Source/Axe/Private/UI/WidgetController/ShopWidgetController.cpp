// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ShopWidgetController.h"

#include "Inventory/Component/InventoryComponent.h"
#include "Inventory/Component/ShopComponent.h"

void UShopWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	Super::SetWidgetControllerParams(Params);
}

void UShopWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	// bind
	ShopComponent->OnInventoryChangedDelegate.AddDynamic(
		this, &UShopWidgetController::OnShopChangedCallback);
}

void UShopWidgetController::TryBuyShopItem(int32 ShopSlot)
{
	bool bCheckCanBuyShopItemClient = ShopComponent->CheckCanBuyShopItemClient(ShopSlot, InventoryComponent);
	if (bCheckCanBuyShopItemClient)
	{
		InventoryComponent->ServerBuyShopItem(ShopComponent, ShopSlot);
	}
}

void UShopWidgetController::OnShopChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
                                                  UItemInstance* OldItemInstance, int32 OldCount)
{
	OnShopChangedInWidgetController.Broadcast(SlotIndex, NewItemInstance, NewCount, OldCount);
}
