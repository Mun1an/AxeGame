// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/AxeInventoryTypes.h"
#include "InventoryComponent.generated.h"

class UItemDefinition;
class AAxeCharacterBase;
class UInventoryProcessor;
class UItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class UInventoryProcessor;

	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeComponent() override;

	AActor* GetOwnerActor() const { return OwnerActor; }
	bool SetOwnerActor(AActor* InOwnerActor);
	AActor* GetAvatarActor() const { return AvatarActor; }
	bool SetAvatarActor(AActor* InAvatarActor);
	UFUNCTION(BlueprintCallable)
	AAxeCharacterBase* GetAxeCharacterOwner() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventorySize() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory)
	UItemInstance* GetItemInstanceByIndex(int32 Index) const;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UItemInstance* AddItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UItemInstance* ItemInstance);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Layout")
	TArray<FGameplayTagContainer> CustomInventorySlots;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	                                 FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface


private:
	UPROPERTY(Replicated)
	FAxeInventoryList InventoryList;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> AvatarActor;
};
