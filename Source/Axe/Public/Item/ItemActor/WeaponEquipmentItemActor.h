// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemActor.h"
#include "GameFramework/Actor.h"
#include "WeaponEquipmentItemActor.generated.h"


UCLASS(Blueprintable, Abstract)
class AXE_API AWeaponEquipmentItemActor : public AEquipmentItemActor
{
	GENERATED_BODY()

public:
	AWeaponEquipmentItemActor();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable)
	virtual UMeshComponent* GetHitTraceMeshComponent() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
};
