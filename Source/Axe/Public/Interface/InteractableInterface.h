// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class AAxeCharacterPlayer;
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
	virtual TObjectPtr<UInteractableComponent> GetInteractableComponent() = 0;

	virtual void OnStartBePreInteracting(AAxeCharacterPlayer* InteractPlayer) = 0;
	virtual void OnEndBePreInteracting(AAxeCharacterPlayer* InteractPlayer) = 0;
};
