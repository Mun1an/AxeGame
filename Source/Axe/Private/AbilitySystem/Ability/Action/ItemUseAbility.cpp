// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Action/ItemUseAbility.h"

#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterPlayer.h"
#include "Interface/InventoryInterface.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Item/DisplayItemActor/DisplayItemActor.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Interface/ItemUseInterface.h"
#include "Item/ItemFragment/ItemFragment_World.h"

void UItemUseAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	UseItemBarSlotIndex = -1;

	const float EventMagnitude = TriggerEventData->EventMagnitude;
	int32 ItemBarIndex = static_cast<int32>(EventMagnitude);
	UseItemBarSlotIndex = ItemBarIndex;
}

void UItemUseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!TriggerEventData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
}

bool UItemUseAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    FGameplayTagContainer* OptionalRelevantTags)
{
	bool bResult = Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);

	bool bUseItemByIndex = UseItemByIndex(UseItemBarSlotIndex);

	return bResult & bUseItemByIndex;
}

bool UItemUseAbility::CheckCanUseItemByIndex(int32 UseItemBarIndex)
{
	UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	const int32 InvSlot = GetInvSlotIndexByUseItemIndex(UseItemBarIndex);
	return InventoryComponent->CheckEntryCanItemUse(InvSlot);
}

bool UItemUseAbility::UseItemByIndex(const int32 UseItemBarIndex)
{
	UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	const int32 InvSlot = GetInvSlotIndexByUseItemIndex(UseItemBarIndex);
	if (!InventoryComponent->CheckEntryCanItemUse(InvSlot))
	{
		return false;
	}
	// 使用物品
	UItemInstance* ItemInstance = InventoryComponent->GetItemInstanceByIndex(InvSlot);
	IItemUseInterface* ItemUseInterface = Cast<IItemUseInterface>(ItemInstance);
	if (!ItemUseInterface)
	{
		return false;
	}
	const bool bUseItemSuccess = ItemUseInterface->OnUseItem();
	if (!bUseItemSuccess)
	{
		return false;
	}
	if (ItemUseInterface->GetIsReduceAfterUse())
	{
		InventoryComponent->RemoveItemByIndex(InvSlot);
	}
	return true;
}

int32 UItemUseAbility::GetInvSlotIndexByUseItemIndex(const int32 UseItemBarIndex)
{
	const UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	TArray<FInventoryEntry> OutEntries;
	InventoryComponent->GetUseItemEntryArray(OutEntries);
	if (OutEntries.IsValidIndex(UseItemBarIndex))
	{
		return OutEntries[UseItemBarIndex].SlotIndex;
	}
	return 0;
}

AActor* UItemUseAbility::CreateItemDisplayActor(const FTransform& Transform, FName AttachSocket)
{
	UWorld* World = GetWorld();
	AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
	const int32 InvSlot = GetInvSlotIndexByUseItemIndex(UseItemBarSlotIndex);
	UInventoryComponent* InventoryComponent = GetOwnerInventoryComponent();
	UItemInstance* ItemInstance = InventoryComponent->GetItemInstanceByIndex(InvSlot);
	if (!ItemInstance)
	{
		return nullptr;
	}
	const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemInstance->GetItemDef());
	const UItemFragment_World* ItemFragment_World = ItemDefinition->FindFragment<UItemFragment_World>();
	if (!ItemFragment_World)
	{
		return nullptr;
	}
	const TSubclassOf<ADisplayItemActor> DisplayItemActorClass = ItemFragment_World->DisplayItemActorClass;
	if (DisplayItemActorClass == nullptr)
	{
		return nullptr;
	}

	USceneComponent* AttachTarget = AxeCharacterOwner->GetRootComponent();
	if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(AxeCharacterOwner))
	{
		AttachTarget = AxeCharacterPlayer->GetRetargetCharacterMesh();
	}

	ADisplayItemActor* ItemActor = World->SpawnActorDeferred<ADisplayItemActor>(
		DisplayItemActorClass, Transform, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ItemActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::SnapToTargetIncludingScale,
	                             AttachSocket);
	ItemActor->FinishSpawning(Transform);

	return ItemActor;
}

UInventoryComponent* UItemUseAbility::GetOwnerInventoryComponent()
{
	if (!OwnerInventoryComponent)
	{
		AAxeCharacterBase* AxeCharacterOwner = GetAxeCharacterOwner();
		if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(AxeCharacterOwner))
		{
			OwnerInventoryComponent = InventoryInterface->GetInventoryComponent();
		}
	}
	return OwnerInventoryComponent;
}
