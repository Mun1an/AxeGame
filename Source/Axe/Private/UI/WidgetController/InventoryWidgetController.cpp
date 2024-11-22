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
}

