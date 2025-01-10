// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */

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
	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UItemFragment* FindItemDefinitionFragment(TSubclassOf<UItemDefinition> ItemDef,
	                                                       TSubclassOf<UItemFragment> FragmentClass);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UItemInstance* CreateItemInstance(UObject* WorldContextObject, TSubclassOf<UItemDefinition> ItemDef);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static UEquipmentItemInstance* CreateEquipmentItemInstanceWithLevel(UObject* WorldContextObject,
	                                                                    TSubclassOf<UEquipmentItemDefinition>
	                                                                    EquipmentItemDef, int32 Level);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AAxeWorldItemActor* CreateWorldItemActor(UObject* WorldContextObject,
	                                                TSubclassOf<AAxeWorldItemActor> ActorClass,
	                                                UItemInstance* ItemInstance,
	                                                const FTransform& Transform, int32 StackCount = 1);
};
