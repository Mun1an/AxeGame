// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ItemActor/WeaponEquipmentItemActor.h"

AWeaponEquipmentItemActor::AWeaponEquipmentItemActor()
{
	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(RootComponent);
}

UMeshComponent* AWeaponEquipmentItemActor::GetHitTraceMeshComponent() const
{
	return StaticMeshComponent;
}
