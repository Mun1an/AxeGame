#include "Inventory/Processor/InventoryProcessor.h"

#include "Inventory/Component/InventoryComponent.h"

UInventoryProcessor::UInventoryProcessor()
{
}

UInventoryComponent* UInventoryProcessor::GetOwningInventory() const
{
	return CastChecked<UInventoryComponent>(GetOuter());
}
