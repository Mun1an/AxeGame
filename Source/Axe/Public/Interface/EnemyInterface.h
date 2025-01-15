// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable, BlueprintType)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IEnemyInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void SetCombatTarget(AActor* NewCombatTarget) = 0;

	UFUNCTION(BlueprintCallable)
	virtual AActor* GetCombatTarget() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetLootXp() const = 0;
};
