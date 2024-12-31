// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_CommonInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UItemFragment_CommonInfo : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_CommonInfo()
	{
		EntryTags = FGameplayTagContainer();
		EntryTags.AddTag(FAxeGameplayTags::Get().Inventory_Entry_Bag);

		ItemTypeTag = FAxeGameplayTags::Get().ItemType_None;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FGameplayTagContainer EntryTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FGameplayTag ItemTypeTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	int32 ItemMaxStackSize = 1;
};
