// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeWorldItemActor.h"
#include "EquipmentRarityDataAsset.h"
#include "ItemSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */

enum class EEquipmentRarity : uint8;
class UEquipmentItemDefinition;
class UEquipmentItemInstance;
class AAxeWorldItemActor;
class UItemDefinition;
class UItemFragment;
class UItemInstance;

UCLASS()
class AXE_API UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static const UItemSubsystem* GetItemSubsystem(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UItemFragment* FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef,
	                                                       TSubclassOf<UItemFragment> FragmentClass);

	// 使用后需要调用FinishItemInstanceCreated
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UItemInstance* CreateItemInstance(UObject* WorldContextObject, TSubclassOf<UItemDefinition> ItemDef);
	
	// 使用后需要调用FinishItemInstanceCreated
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UEquipmentItemInstance* CreateEquipmentItemInstance(UObject* WorldContextObject,
	                                                           TSubclassOf<UEquipmentItemDefinition>
	                                                           EquipmentItemDef, int32 Level,
	                                                           EEquipmentRarity EquipmentRarity);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AAxeWorldItemActor* CreateWorldItemActor(UObject* WorldContextObject,
	                                                const FTransform& Transform,
	                                                UItemInstance* ItemInstance,
	                                                int32 StackCount = 1);

	UFUNCTION(BlueprintCallable)
	static FEquipmentRarityInfo GetEquipmentRarityInfo(UObject* WorldContextObject, EEquipmentRarity EquipmentRarity);
};
