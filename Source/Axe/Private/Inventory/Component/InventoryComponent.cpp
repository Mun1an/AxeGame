// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/InventoryComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/Processor/InventoryProcessor.h"
#include "Item/Instance/ItemInstance.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
                                                                                       SlotArray(this)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::InitializeComponent()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (FGameplayTagContainer& TagContainer : CustomInventorySlots) 
		{
			CreateInventorySlot(TagContainer);
		}
	}

	Super::InitializeComponent();
}

void UInventoryComponent::ForEachProcessor(ForEachProcessorFunc Func)
{
	for (UInventoryProcessor* Processor : ProcessorArray)
	{
		if (IsValid(Processor))
		{
			Func(Processor);
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ForEachProcessor([](UInventoryProcessor* Processor)
	{
		Processor->OnInventoryBeginPlay();
	});
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ForEachProcessor([&EndPlayReason](UInventoryProcessor* Processor)
	{
		Processor->OnInventoryEndPlay(EndPlayReason);
	});

	Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::CreateInventorySlot(const FGameplayTagContainer& SlotTags)
{
	FInventoryItemSlot Slot;
	Slot.SlotId = IdCounter;
	Slot.SlotTags = SlotTags;
	Slot.ParentInventory = this;

	SlotArray.Slots.Add(Slot);

	IdCounter++;
	SlotArray.MarkItemDirty(Slot);
	SlotArray.MarkArrayDirty();

	PostInventoryUpdate();
}

void UInventoryComponent::PostInventoryUpdate()
{
	// 广播
}

UInventoryProcessor* UInventoryComponent::AddProcessor(TSubclassOf<UInventoryProcessor> ProcessorClass,
                                                       FGameplayTagContainer ProcessorTags)
{
	UInventoryProcessor* NewProcessor = NewObject<UInventoryProcessor>(this, ProcessorClass);
	NewProcessor->FragmentTags = ProcessorTags;
	ProcessorArray.Add(NewProcessor);

	NewProcessor->OnInventoryInit();
	if (HasBegunPlay())
	{
		NewProcessor->OnInventoryBeginPlay();
	}

	return NewProcessor;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, SlotArray);
	DOREPLIFETIME(UInventoryComponent, ProcessorArray);
}

// bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
// 	FReplicationFlags* RepFlags)
// {
// 	bool WroteSomething = false;
// 	WroteSomething |= Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//
// 	for (const FInventoryItemSlot& Slot : SlotArray) 
// 	{
// 		if (IsValid(Slot.ItemInstance))
// 		{
// 			WroteSomething |= Channel->ReplicateSubobject(Slot.ItemInstance, *Bunch, *RepFlags);
// 			WroteSomething |= Slot.ItemInstance->ReplicateSubobjects(Channel, Bunch, RepFlags);
// 		}
// 	}
// 	return WroteSomething;
// }
