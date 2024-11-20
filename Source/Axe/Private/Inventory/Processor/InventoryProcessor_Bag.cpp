#include "Inventory/Processor/InventoryProcessor_Bag.h"

#include "Inventory/Component/InventoryComponent.h"

UInventoryProcessor_Bag::UInventoryProcessor_Bag():BagSlotCount(1)
{
}

void UInventoryProcessor_Bag::OnInventoryBeginPlay_Implementation()
{
	Super::OnInventoryBeginPlay_Implementation();
	if (GetOwningInventory()->GetOwnerRole() == ROLE_Authority)
	{
		for (int32 i = 0; i < BagSlotCount; i++)
		{
			GetOwningInventory()->CreateInventorySlot(BagSlotTags);
		}
	}
}
