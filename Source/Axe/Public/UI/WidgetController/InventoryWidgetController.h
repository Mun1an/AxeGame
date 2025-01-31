// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "InventoryWidgetController.generated.h"

class UInventoryEntryDefaultIconDataAsset;
class UItemInstance;
class UInventoryComponent;
/**
 * 
 */
UENUM(BlueprintType)
enum class EInventoryBoxOptionSelection : uint8
{
	None,
	Drop,
	Destroy,
	Sell
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInventoryChangedInWidgetController, int32, SlotIndex, UItemInstance*,
                                              ItemInstance,
                                              int32, NewCount, int32, OldCount);

UCLASS(BlueprintType, Blueprintable)
class AXE_API UInventoryWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()

public:
	UInventoryWidgetController();

	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params) override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryChangedInWidgetController OnInventoryChangedInWidgetController;

	UFUNCTION(BlueprintCallable)
	void TrySwapItemSlot(int32 FromSlot, int32 ToSlot);

protected:
	UFUNCTION()
	void OnInventoryChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
	                                UItemInstance* OldItemInstance, int32 OldCount);

	UPROPERTY(BlueprintReadOnly, Category="Inventory|WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="InventoryEntryDefaultIconDataAsset")
	TObjectPtr<UInventoryEntryDefaultIconDataAsset> InventoryEntryDefaultIconDataAsset;

	UPROPERTY(BlueprintReadWrite, Category="Inventory|Option")
	TArray<EInventoryBoxOptionSelection> InventoryBoxOptionSelections;
};
