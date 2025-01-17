// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFunctionLibrary.h"

#include "AssetManager/AxeAssetManager.h"
#include "AssetManager/AxeGameData.h"
#include "Item/AxeWorldItemActor.h"
#include "Item/Component/ItemComponent.h"
#include "Item/Instance/EquipmentItemInstance.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment.h"

const UItemSubsystem* UItemFunctionLibrary::GetItemSubsystem(UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	return World->GetGameInstance()->GetSubsystem<UItemSubsystem>();
}

const UItemFragment* UItemFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef,
                                                                      TSubclassOf<UItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

UItemInstance* UItemFunctionLibrary::CreateItemInstance(UObject* WorldContextObject,
                                                        TSubclassOf<UItemDefinition> ItemDef)
{
	if (!ItemDef)
	{
		return nullptr;
	}
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	const UItemDefinition* ItemDefinition = ItemDef->GetDefaultObject<UItemDefinition>();
	const UClass* InstanceClass = ItemDefinition->ItemInstanceClass;
	UItemInstance* ItemInstance = NewObject<UItemInstance>(World, InstanceClass);
	ItemInstance->SetItemDef(ItemDef);
	ItemInstance->OnItemInstanceCreated();
	return ItemInstance;
}

UEquipmentItemInstance* UItemFunctionLibrary::CreateEquipmentItemInstance(UObject* WorldContextObject,
                                                                          TSubclassOf<UEquipmentItemDefinition>
                                                                          EquipmentItemDef,
                                                                          int32 Level, EEquipmentRarity EquipmentRarity)
{
	UItemInstance* ItemInstance = CreateItemInstance(WorldContextObject, EquipmentItemDef);
	UEquipmentItemInstance* EquipmentItemInstance = Cast<UEquipmentItemInstance>(ItemInstance);
	if (!EquipmentItemInstance)
	{
		return nullptr;
	}
	EquipmentItemInstance->InitEquipmentItemInstanceInfo(Level, EquipmentRarity);
	return EquipmentItemInstance;
}

AAxeWorldItemActor* UItemFunctionLibrary::CreateWorldItemActor(UObject* WorldContextObject,
                                                               const FTransform& Transform,
                                                               UItemInstance* ItemInstance,
                                                               int32 StackCount)
{
	const TSubclassOf<AAxeWorldItemActor> ItemActorClass = UAxeAssetManager::GetSubclass(
		UAxeGameData::Get().WorldItemActorClass);
	if (!ItemActorClass)
	{
		return nullptr;
	}
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	AAxeWorldItemActor* ItemActor = World->SpawnActorDeferred<AAxeWorldItemActor>(
		ItemActorClass, Transform, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UItemComponent* ItemActorItemComponent = ItemActor->GetItemComponent();
	ItemActorItemComponent->SetItemInstance(ItemInstance);
	ItemActorItemComponent->SetStackCount(StackCount);

	ItemActor->FinishSpawning(Transform);

	ItemActor->InitDisplayItemActor();

	return ItemActor;
}

FEquipmentRarityInfo UItemFunctionLibrary::GetEquipmentRarityInfo(UObject* WorldContextObject,
                                                                  EEquipmentRarity EquipmentRarity)
{
	const UItemSubsystem* ItemSubsystem = GetItemSubsystem(WorldContextObject);
	if (ItemSubsystem)
	{
		UEquipmentRarityDataAsset* RarityDataAsset = ItemSubsystem->GetEquipmentRarityDataAsset();
		if (RarityDataAsset)
		{
			return RarityDataAsset->GetEquipmentRarityInfo(EquipmentRarity);
		}
	}
	return FEquipmentRarityInfo::Empty;
}
