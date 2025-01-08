// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "Item/ItemActor/EquipmentItemActor.h"
#include "EquipmentItemDefinition.generated.h"

class UGameplayEffect;
class UEquipmentItemInstance;
//
USTRUCT(BlueprintType)
struct FAxeEquipmentActorToSpawn
{
	GENERATED_BODY()

	FAxeEquipmentActorToSpawn()
	{
	}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AEquipmentItemActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

USTRUCT(BlueprintType)
struct FEquipmentAttrInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentArmor = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	float EquipmentMaxHealth = 0.0f;
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class AXE_API UEquipmentItemDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	UEquipmentItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UPROPERTY(EditDefaultsOnly, Category=Equipment)
	// TArray<TObjectPtr<const UAxeAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FAxeEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attr")
	FEquipmentAttrInfo EquipmentAttrInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<UGameplayEffect> EquipmentEffect;
};
