// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Component/InventoryComponent.h"

#include "Character/AxeCharacterBase.h"
#include "Engine/ActorChannel.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Inventory/Processor/InventoryProcessor.h"
#include "Inventory/Processor/InventoryProcessor_Bag.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, OwnerActor);
	DOREPLIFETIME(UInventoryComponent, AvatarActor);
}

bool UInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                              FReplicationFlags* RepFlags)
{
	bool WroteSomething = false;
	WroteSomething |= Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryEntry& Entry : InventoryList.Entries)
		{
			UItemInstance* Instance = Entry.Instance;
			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UInventoryComponent::OnInventoryItemChanged(int32 SlotIndex, UItemInstance* ItemInstance, int32 NewCount,
                                                 int32 OldCount)
{
	OnInventoryChanged.Broadcast(SlotIndex, ItemInstance, NewCount, OldCount);
}

bool UInventoryComponent::SetOwnerActor(AActor* InOwnerActor)
{
	OwnerActor = InOwnerActor;
	return true;
}

bool UInventoryComponent::SetAvatarActor(AActor* InAvatarActor)
{
	AvatarActor = InAvatarActor;
	return true;
}

AAxeCharacterBase* UInventoryComponent::GetAxeCharacterOwner() const
{
	if (AActor* LocalAvatarActor = GetAvatarActor())
	{
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalAvatarActor))
		{
			return AxeCharacterBase;
		}
	}
	else
	{
		AActor* LocalOwnerActor = GetOwner();
		if (AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(LocalOwnerActor))
		{
			return AxeCharacterBase;
		}
	}
	return nullptr;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// init slot
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (int i = 0; i < InventoryEntryInitSize; ++i)
		{
			InventoryList.AddEntry();
		}
	}
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

int32 UInventoryComponent::GetInventoryEntriesSize() const
{
	return InventoryList.Entries.Num();
}

TArray<UItemInstance*> UInventoryComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UItemInstance* UInventoryComponent::GetItemInstanceByIndex(int32 Index) const
{
	return InventoryList.GetItemInstanceByIndex(Index);
}

UItemInstance* UInventoryComponent::AddItemDefinition(TSubclassOf<UItemDefinition> ItemDef, int32 StackCount,
                                                      int32 Index)
{
	if (ItemDef == nullptr)
	{
		return nullptr;
	}
	UItemInstance* ItemInstance = NewObject<UItemInstance>(GetOwner());
	ItemInstance->SetItemDef(ItemDef);
	AddItemInstance(ItemInstance, StackCount, Index);
	return ItemInstance;
}

void UInventoryComponent::AddItemInstance(UItemInstance* ItemInstance, int32 StackCount, int32 Index)
{
	InventoryList.AddItem(ItemInstance, StackCount, Index);

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UInventoryComponent::AddInventoryEntry()
{
	InventoryList.AddEntry();
}

