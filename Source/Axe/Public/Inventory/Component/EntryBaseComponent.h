// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/AxeInventoryTypes.h"
#include "EntryBaseComponent.generated.h"

class UGameplayEffect;
class UItemDefinition;
class AAxeCharacterBase;
class UItemInstance;
class UTexture2D;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnInventoryChangedDelegate, int32, SlotIndex, UItemInstance*,
                                              NewItemInstance,
                                              int32, NewCount, UItemInstance*, OldItemInstance, int32, OldCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSendInventoryItemUIMessage, UTexture2D*, Texture,
                                               FText, ItemName, int32, StackCount);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UEntryBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend struct FAxeInventoryList;

	UEntryBaseComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

	//~End of UObject interface
	virtual void InitInventoryEntry();
	//
	FOnInventoryChangedDelegate OnInventoryChangedDelegate;
	FOnSendInventoryItemUIMessage OnSendInventoryItemUIMessage;
	//

	AActor* GetOwnerActor() const { return OwnerActor; }
	bool SetOwnerActor(AActor* InOwnerActor);
	AActor* GetAvatarActor() const { return AvatarActor; }
	bool SetAvatarActor(AActor* InAvatarActor);
	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* GetAxeCharacterOwner() const;

	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetInventoryEntriesSize() const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	TArray<UItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void GetAllEntryArray(TArray<FInventoryEntry>& OutEntries) const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	UItemInstance* GetItemInstanceByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	FInventoryEntry& GetInventoryEntryByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool AddItemInstance(UItemInstance* ItemInstance, int32 StackCount = 1, int32 Index = -1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool RemoveItemByIndex(int32 Index, int32 RemoveCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool SwapItemBySlots(int32 FromSlot, int32 ToSlot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void DropItemBySlot(int32 Slot);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool SellItemBySlot(int32 Slot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSwapItemBySlots(int32 FromSlot, int32 ToSlot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRemoveItemBySlot(int32 Slot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItemBySlot(int32 Slot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSellItemBySlot(int32 Slot);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddInventoryEntry();

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void SendItemUIMessage(TSubclassOf<UItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	void RefreshInventoryItemEntryChange();

	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool CheckEntryHasTag(const FGameplayTag CheckTag, FInventoryEntry& Entry);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool CheckEntryHasItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool CheckEntryCanItemUse(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool CheckCanAddItemInstance(UItemInstance* ItemInstance, int32 ItemCount, FGameplayTag SlotTag);

protected:
	UFUNCTION(Client, Reliable)
	void ClientSendItemUIMessage(UTexture2D* Texture, const FText& ItemName, int32 StackCount);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	virtual void OnInventoryItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance, int32 NewCount,
	                                    UItemInstance* OldItemInstance, int32 OldCount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InventoryEntryInitSize = 16;

	UPROPERTY(Replicated)
	FAxeInventoryList InventoryList;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> AvatarActor;

private:
};
