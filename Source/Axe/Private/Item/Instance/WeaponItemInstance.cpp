// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Instance/WeaponItemInstance.h"

#include "Character/AxeCharacterPlayer.h"
#include "Item/ItemActor/WeaponEquipmentItemActor.h"

UWeaponItemInstance::UWeaponItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UWeaponItemInstance::SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn)
{
	Super::SpawnEquipmentActors(ActorsToSpawn);
}

void UWeaponItemInstance::DestroyEquipmentActors()
{
	Super::DestroyEquipmentActors();
}

void UWeaponItemInstance::OnEquipped()
{
	Super::OnEquipped();

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn());
	if (AxeCharacterPlayer)
	{
		if (SpawnedActors.Num() > 0)
		{
			AActor* Actor = SpawnedActors[0];
			AWeaponEquipmentItemActor* WeaponEquipmentItemActor = Cast<AWeaponEquipmentItemActor>(Actor);
			AxeCharacterPlayer->SetWeaponEquipmentActor(WeaponEquipmentItemActor);
		}
		if (SpawnedActors.Num() > 1)
		{
			AActor* Actor = SpawnedActors[1];
			AWeaponEquipmentItemActor* WeaponEquipmentItemActor = Cast<AWeaponEquipmentItemActor>(Actor);
			AxeCharacterPlayer->SetWeaponEquipmentActorSecondary(WeaponEquipmentItemActor);
		}
	}
}

void UWeaponItemInstance::OnUnequipped()
{
	Super::OnUnequipped();

	if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetPawn()))
	{
		AxeCharacterPlayer->SetWeaponEquipmentActor(nullptr);
		AxeCharacterPlayer->SetWeaponEquipmentActorSecondary(nullptr);
	}
}
