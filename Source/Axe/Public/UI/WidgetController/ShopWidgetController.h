// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetController.h"
#include "OverlayWidgetController.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "ShopWidgetController.generated.h"

class UEntryBaseComponent;
class UShopComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UShopWidgetController : public UInventoryWidgetController
{
	GENERATED_BODY()

public:
	UShopWidgetController();

	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params) override;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UShopComponent* GetShopComponent() const { return ShopComponent; }
	void SetShopComponent(UShopComponent* InShopComponent) { ShopComponent = InShopComponent; }

	UFUNCTION(BlueprintCallable, Category="Shop")
	void TryBuyShopItem(int32 ShopSlot);

	UFUNCTION(BlueprintCallable, Category="Shop")
	void TryRefreshShopItem();

	UPROPERTY(BlueprintAssignable, Category="Shop")
	FOnInventoryChangedInWidgetController OnShopChangedInWidgetController;

	UPROPERTY(BlueprintAssignable, Category="GAS|GoldCoin")
	FOnPlayerStatChangedSignature OnGoldCoinCountChangedDelegate;

protected:
	UFUNCTION()
	void OnShopChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
	                           UItemInstance* OldItemInstance, int32 OldCount);

	void OnGoldCoinCountChanged(int32 NewValue, int32 OldValue);

	UPROPERTY(BlueprintReadOnly)
	UShopComponent* ShopComponent = nullptr;
};
