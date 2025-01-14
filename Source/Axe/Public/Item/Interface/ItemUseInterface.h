// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemUseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IItemUseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanUseItem();

	virtual bool OnUseItem();
	
	virtual bool GetIsReduceAfterUse();
};
