// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Item/AxeItemInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UItemInstance : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FItemTableRowFragmentArrayHandle GetItemStructsHandle() const
	{
		return ItemStructsHandle;
	}

	virtual void SetItemStructsHandle(FItemTableRowFragmentArrayHandle InItemStructsHandle)
	{
		ItemStructsHandle = InItemStructsHandle;
	}

	bool operator==(const UItemInstance& Other) const
	{
		return ItemStructsHandle.Data.Get() == Other.ItemStructsHandle.Data.Get();
	}

	bool operator!=(const UItemInstance& Other) const
	{
		return !(*this == Other);
	}

	//
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item")
	int32 StackSize = 1;

protected:
	UPROPERTY()
	FItemTableRowFragmentArrayHandle ItemStructsHandle;
};
