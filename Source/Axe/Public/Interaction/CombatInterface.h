// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UActionCombatComponent;
class UComboActionComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual UComboActionComponent* GetComboActionComponent() const = 0;
	virtual UActionCombatComponent* GetActionCombatComponent() const = 0;

	virtual UStaticMeshComponent* GetWeaponComponent() const = 0;
	virtual UStaticMeshComponent* GetWeaponSecondaryComponent() const = 0;

};
