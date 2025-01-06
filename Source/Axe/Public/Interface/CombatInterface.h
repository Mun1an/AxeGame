// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class AWeaponEquipmentItemActor;
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UComboActionComponent* GetComboActionComponent() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UActionCombatComponent* GetActionCombatComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AWeaponEquipmentItemActor* GetWeaponEquipmentActor() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AWeaponEquipmentItemActor* GetWeaponEquipmentActorSecondary() const;
};
