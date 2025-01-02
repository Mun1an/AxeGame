// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDeadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IDeadInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual bool IsDead() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const = 0;

	UFUNCTION()
	virtual void OnDead() = 0;
};
