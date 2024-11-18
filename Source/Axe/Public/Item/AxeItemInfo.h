// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "Engine/DataTable.h"
#include "AxeItemInfo.generated.h"

enum class EItemType : uint8;

USTRUCT()
struct FItemTableRowFragment
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FItemTableRowFragment_UI : public FItemTableRowFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;
};

USTRUCT(BlueprintType)
struct FItemTableRowFragment_World : public FItemTableRowFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FItemTableRowFragmentArray
{
	GENERATED_BODY()

	FItemTableRowFragmentArray()
	{
	}

	FItemTableRowFragmentArray(const FItemTableRowFragmentArray& Other): ItemTags(Other.ItemTags),
	                                                                     ItemFragments(Other.ItemFragments)
	{
	}

	FItemTableRowFragmentArray(const FGameplayTagContainer& InSlotTags, const TArray<FInstancedStruct>& InItemFragments)
		: ItemTags(InSlotTags), ItemFragments(InItemFragments)
	{
	}

	bool IsValid() const
	{
		return !ItemFragments.IsEmpty();
	}

	bool operator==(const FItemTableRowFragmentArray& Other) const
	{
		return ItemTags == Other.ItemTags && ItemFragments == Other.ItemFragments;
	}

	bool operator!=(const FItemTableRowFragmentArray& Other) const
	{
		return !(*this == Other);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BaseStruct="/Script/Axe.ItemTableRowFragment", ExcludeBaseStruct),
		Category = "Item")
	TArray<FInstancedStruct> ItemFragments;
};


USTRUCT(BlueprintType)
struct FItemTableRowFragmentArrayHandle
{
	GENERATED_BODY()

	FItemTableRowFragmentArrayHandle(): Data(nullptr)
	{
	}

	FItemTableRowFragmentArrayHandle(FItemTableRowFragmentArray* InData): Data(InData)
	{
	}

	FItemTableRowFragmentArrayHandle(const FItemTableRowFragmentArrayHandle& Other): Data(Other.Data)
	{
	}

	bool IsValid() const
	{
		return Data.IsValid();
	}

	bool FindFragment_UI(FItemTableRowFragment_UI& OutFragment);
	bool FindFragment_World(FItemTableRowFragment_World& OutFragment);

	TSharedPtr<FItemTableRowFragmentArray> Data;
};

/**
 * TableRowDefine
 */
USTRUCT(BlueprintType)
struct FAxeItemTableRowDefine : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FItemTableRowFragmentArray ItemInfos;
};
