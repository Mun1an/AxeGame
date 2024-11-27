// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighLight.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UHighLight : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IHighLight
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void HighlightActor() = 0;

	UFUNCTION(BlueprintCallable)
	virtual void UnHighlightActor() = 0;
};
