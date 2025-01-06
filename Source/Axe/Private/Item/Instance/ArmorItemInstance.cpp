// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Instance/ArmorItemInstance.h"

#include "Character/AxeCharacterPlayer.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment_ModularCharacterMesh.h"

UArmorItemInstance::UArmorItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UArmorItemInstance::SpawnEquipmentActors(const TArray<FAxeEquipmentActorToSpawn>& ActorsToSpawn)
{
	Super::SpawnEquipmentActors(ActorsToSpawn);
}

void UArmorItemInstance::DestroyEquipmentActors()
{
	Super::DestroyEquipmentActors();
}

void UArmorItemInstance::OnEquipped()
{
	Super::OnEquipped();

	const UItemDefinition* ItemDefCDO = GetDefault<UItemDefinition>(GetItemDef());
	
	const UItemFragment_ModularCharacterMesh* ModularMeshFragment = ItemDefCDO->FindFragment<
		UItemFragment_ModularCharacterMesh>();
	UModularCharacterComponent* ModularCharacterComp = GetModularCharacterComponent();
	if (ModularMeshFragment->ModularCharacterMeshInfos.Num() > 0)
	{
		for (const FModularCharacterMeshInfo& ModularCharacterMeshInfo : ModularMeshFragment->
		 ModularCharacterMeshInfos)
		{
			ModularCharacterComp->SetModularMesh(
				ModularCharacterMeshInfo.ModularMeshEnum, ModularCharacterMeshInfo.ModularMesh, true
			);
		}

		ModularCharacterComp->OnAfterUpdateModularMesh();
	}
}

void UArmorItemInstance::OnUnequipped()
{
	Super::OnUnequipped();

	const UItemDefinition* ItemDefCDO = GetDefault<UItemDefinition>(GetItemDef());
	
	const UItemFragment_ModularCharacterMesh* ModularMeshFragment = ItemDefCDO->FindFragment<
		UItemFragment_ModularCharacterMesh>();
	UModularCharacterComponent* ModularCharacterComp = GetModularCharacterComponent();
	if (ModularMeshFragment->ModularCharacterMeshInfos.Num() > 0)
	{
		for (const FModularCharacterMeshInfo& ModularCharacterMeshInfo : ModularMeshFragment->
		 ModularCharacterMeshInfos)
		{
			ModularCharacterComp->SetModularMesh(
				ModularCharacterMeshInfo.ModularMeshEnum, nullptr, false
			);
		}
		ModularCharacterComp->OnAfterUpdateModularMesh();
	}
}

UModularCharacterComponent* UArmorItemInstance::GetModularCharacterComponent()
{
	if (!ModularCharacterComponent)
	{
		APawn* Pawn = GetPawn();
		if (AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(Pawn))
		{
			ModularCharacterComponent = AxeCharacterPlayer->GetModularCharacterComponent();
		}
	}
	return ModularCharacterComponent;
}
