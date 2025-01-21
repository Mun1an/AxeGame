// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_Shop.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UItemFragment_Shop : public UItemFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	int32 DefaultPrice = 0;
};
