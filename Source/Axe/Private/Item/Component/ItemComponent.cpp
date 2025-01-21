// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Engine/ActorChannel.h"
#include "Item/AxeWorldItemActor.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/ItemInfoWidgetComponent.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemComponent, ItemDef);
	DOREPLIFETIME(UItemComponent, ItemInstance);
	DOREPLIFETIME(UItemComponent, StackCount);
}

bool UItemComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                         FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (ItemInstance && IsValid(ItemInstance))
	{
		WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void UItemComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		if (IsValid(ItemInstance))
		{
			AddReplicatedSubObject(ItemInstance);
		}
	}
}

void UItemComponent::SetItemInstance(UItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
	ItemDef = ItemInstance->GetItemDef();

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority() && !ItemInstance && ItemDef)
	{
		ItemInstance = UItemFunctionLibrary::CreateItemInstance(GetWorld(), ItemDef);
		ItemInstance->FinishItemInstanceCreated();
	}
}
