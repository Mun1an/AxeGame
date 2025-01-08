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

	WeaponTrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponTrailNiagaraComponent"));
	WeaponTrailNiagaraComponent->SetupAttachment(StaticMeshComponent, WeaponTrailAttachSocketName);
	WeaponTrailNiagaraComponent->SetAutoActivate(false);
}

UMeshComponent* AWeaponEquipmentItemActor::GetHitTraceMeshComponent() const
{
	return StaticMeshComponent;
}

void AWeaponEquipmentItemActor::SetWeaponTrail(bool bEnable)
{
	WeaponTrailEnableCount += bEnable ? 1 : -1;
	WeaponTrailEnableCount = FMath::Max(WeaponTrailEnableCount, 0);
	if (WeaponTrailEnableCount > 0)
	{
		WeaponTrailNiagaraComponent->Activate();
	}
	else if (WeaponTrailEnableCount <= 0)
	{
		WeaponTrailNiagaraComponent->Deactivate();
	}
}

void AWeaponEquipmentItemActor::ResetWeaponTrail()
{
	WeaponTrailEnableCount = 0;
	WeaponTrailNiagaraComponent->Deactivate();
}

void AWeaponEquipmentItemActor::BeginPlay()
{
	Super::BeginPlay();
	WeaponTrailNiagaraComponent->SetVariableFloat(WeaponTrailNiagaraWidthVariableName, WeaponTrailWidth);
	WeaponTrailNiagaraComponent->Deactivate();
}
