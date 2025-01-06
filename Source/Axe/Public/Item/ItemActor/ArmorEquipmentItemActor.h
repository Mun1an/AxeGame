// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemActor.h"
#include "GameFramework/Actor.h"
#include "ArmorEquipmentItemActor.generated.h"


UCLASS(Blueprintable, Abstract)
class AXE_API AArmorEquipmentItemActor : public AEquipmentItemActor
{
	GENERATED_BODY()

public:
	AArmorEquipmentItemActor();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComponent;
};
