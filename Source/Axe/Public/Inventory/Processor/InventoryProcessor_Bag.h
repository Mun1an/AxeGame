#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryProcessor.h"
#include "InventoryProcessor_Bag.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew, DefaultToInstanced)
class AXE_API UInventoryProcessor_Bag : public UInventoryProcessor
{
	GENERATED_BODY()

public:
	UInventoryProcessor_Bag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 BagSlotCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FGameplayTagContainer BagSlotTags;

	virtual void OnInventoryBeginPlay_Implementation() override;
};
