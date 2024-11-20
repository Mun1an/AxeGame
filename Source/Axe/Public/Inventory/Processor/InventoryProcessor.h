#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "InventoryProcessor.generated.h"

class UInventoryComponent;

UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew, DefaultToInstanced)
class AXE_API UInventoryProcessor : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UInventoryProcessor();

	UFUNCTION(BlueprintPure, Category="Inventory")
	UInventoryComponent* GetOwningInventory() const;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer.AppendTags(FragmentTags);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Processor")
	FGameplayTagContainer FragmentTags;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryInit();
	virtual void OnInventoryInit_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryBeginPlay();
	virtual void OnInventoryBeginPlay_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryEndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void OnInventoryEndPlay_Implementation(const EEndPlayReason::Type EndPlayReason) {}
};
