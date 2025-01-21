// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntryBaseComponent.h"
#include "Components/ActorComponent.h"
#include "Inventory/AxeInventoryTypes.h"
#include "InventoryComponent.generated.h"

class UShopComponent;
class UGameplayEffect;
class UItemDefinition;
class AAxeCharacterBase;
class UItemInstance;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEquipmentChangedDelegate, int32, SlotIndex, UItemInstance*,
                                              ItemInstance, UItemInstance*, OldItemInstance, FGameplayTagContainer,
                                              SlotTags);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UInventoryComponent : public UEntryBaseComponent
{
	GENERATED_BODY()

public:
	friend struct FAxeInventoryList;

	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitInventoryEntry() override;
	//
	FOnEquipmentChangedDelegate OnEquipmentItemChangedDelegate;
	//
	UFUNCTION(BlueprintCallable, Category=Inventory)
	void GetEquipmentEntryArray(TArray<FInventoryEntry>& OutEntries) const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void GetUseItemEntryArray(TArray<FInventoryEntry>& OutEntries) const;

	//
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory|Shop")
	void ServerBuyShopItem(UShopComponent* ShopComponent, int32 ShopSlot);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnInventoryItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
	                                    UItemInstance* OldItemInstance, int32 OldCount) override;
	void OnEquipmentItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, UItemInstance* OldItemInstance,
	                            FGameplayTagContainer SlotTags);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UseBarEntryInitSize = 4;
};
