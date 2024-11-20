// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/AxeInventoryTypes.h"
#include "InventoryComponent.generated.h"

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

	using ForEachProcessorFunc = TFunctionRef<void(UInventoryProcessor*)>;
	void ForEachProcessor(ForEachProcessorFunc Func);
	
	void CreateInventorySlot(const FGameplayTagContainer& SlotTags);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	void PostInventoryUpdate();

	// Processor
	UInventoryProcessor* AddProcessor(TSubclassOf<UInventoryProcessor> ProcessorClass,
	                                  FGameplayTagContainer ProcessorTags = {});


	UPROPERTY(Replicated)
	FAxeInventoryItemSlotArray SlotArray;

	UPROPERTY(EditAnywhere, Instanced, Category="Inventory", Replicated)
	TArray<TObjectPtr<UInventoryProcessor>> ProcessorArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Layout")
	TArray<FGameplayTagContainer> CustomInventorySlots;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
private:
	int32 IdCounter;
};
