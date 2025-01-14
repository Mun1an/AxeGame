// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/Interface/ItemUseInterface.h"
#include "UseItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UUseItemInstance : public UItemInstance, public IItemUseInterface
{
	GENERATED_BODY()
public:
	virtual bool CanUseItem() override;
	virtual bool OnUseItem() override;
	virtual bool GetIsReduceAfterUse() override;
};
