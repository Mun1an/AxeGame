// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryEntryDefaultIconDataAsset.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FInventoryEntryDefaultIconStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> InventoryEntryDefaultIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InventoryEntryTag;
};

/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UInventoryEntryDefaultIconDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInventoryEntryDefaultIconStruct> InventoryEntryDefaultIconInfo;
};
