// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Character/AxeCharacterPlayer.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Item/Instance/ItemInstance.h"
#include "PlayerState/AxePlayerState.h"

void UInventoryWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	Super::SetWidgetControllerParams(Params);

	check(AxePlayerState)
	InventoryComponent = AxePlayerState->GetInventoryComponent();

	// bind
	InventoryComponent->OnInventoryChangedDelegate.AddDynamic(
		this, &UInventoryWidgetController::OnInventoryChangedCallback);
}

void UInventoryWidgetController::TrySwapItemSlot(int32 FromSlot, int32 ToSlot)
{
	InventoryComponent->ServerSwapItemBySlots(FromSlot, ToSlot);
}

void UInventoryWidgetController::OnInventoryChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance,
                                                            int32 NewCount,
                                                            UItemInstance* OldItemInstance,
                                                            int32 OldCount)
{
	OnInventoryChangedInWidgetController.Broadcast(SlotIndex, NewItemInstance, NewCount, OldCount);
}
