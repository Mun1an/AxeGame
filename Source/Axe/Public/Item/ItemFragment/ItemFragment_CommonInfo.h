// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	int32 ItemMaxStackSize = 1;
};
