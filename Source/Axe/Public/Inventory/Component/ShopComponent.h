// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntryBaseComponent.h"
#include "Components/ActorComponent.h"
#include "ShopComponent.generated.h"


struct FShopItemDataInfo;
class UShopItemDataAsset;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UShopComponent : public UEntryBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShopComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = Shop)
	bool RandomSpawnShopItem();

	UFUNCTION(BlueprintCallable, Category = Shop)
	UItemInstance* CreateShopItemInstance(const FShopItemDataInfo& ShopItemDataInfo);

	int32 GetShopItemPrice(UItemInstance* ItemInstance, float PriceRate);

	UFUNCTION(BlueprintCallable, Category = Shop)
	int32 GetShopItemPriceWithCount(int32 Slot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Shop)
	bool BuyShopItem(int32 ShopSlot, UEntryBaseComponent* BuyerInventory);

	UFUNCTION(BlueprintCallable, Category = Shop)
	bool CheckCanBuyShopItemByClient(int32 ShopSlot, UEntryBaseComponent* BuyerInventory);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Shop)
	bool RefreshShopItem(UEntryBaseComponent* BuyerInventory);

	UFUNCTION(BlueprintCallable, Category = Shop)
	bool CheckCanRefreshShopItemByClient(UEntryBaseComponent* BuyerInventory);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RefreshShopItemCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UShopItemDataAsset> ShopItemDataAsset;
};
