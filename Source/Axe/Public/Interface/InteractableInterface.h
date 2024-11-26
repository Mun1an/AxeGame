// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

struct FInteractionOption;
class UInteractableComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void GetInteractionOptions(FInteractionOption& OutOptions);
};
