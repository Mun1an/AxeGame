// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */

struct FItemTableRowFragment_World;
struct FItemTableRowFragment_UI;
class UItemInstance;

UCLASS()
class AXE_API UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ItemFunctionLibrary|Item")
	static bool FindFragment_UI(UItemInstance* ItemInstance, FItemTableRowFragment_UI& OutFragmentStruct);
	UFUNCTION(BlueprintCallable, Category="ItemFunctionLibrary|Item")
	static bool FindFragment_World(UItemInstance* ItemInstance, FItemTableRowFragment_World& OutFragmentStruct);
};
