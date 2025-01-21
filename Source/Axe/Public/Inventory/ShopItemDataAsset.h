// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ShopItemDataAsset.generated.h"


class UItemDefinition;

USTRUCT(Blueprintable, BlueprintType)
struct FShopItemDataInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UItemDefinition> ItemDef = nullptr;

	// 物品默认最大价格倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDefaultPriceRate = 4;
	// 物品默认最小价格倍率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinDefaultPriceRate = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinItemCount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxItemCount = 1;
};

/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UShopItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FShopItemDataInfo> ShopItemDataInfos;
};
