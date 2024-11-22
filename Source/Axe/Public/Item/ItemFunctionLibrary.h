// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */

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
};
