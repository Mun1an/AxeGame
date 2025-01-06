// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ModularEquipmentItemActor.h"

#include "Item/ItemFunctionLibrary.h"
#include "Item/Component/ItemComponent.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"
#include "Item/ItemFragment/ItemFragment_ModularCharacterMesh.h"

AModularEquipmentItemActor::AModularEquipmentItemActor()
{
	ModularSMList = {};

	HandleModularSkeletalMeshComponent(ModularSM_0, "ModularSM_0");
	HandleModularSkeletalMeshComponent(ModularSM_1, "ModularSM_1");
	HandleModularSkeletalMeshComponent(ModularSM_2, "ModularSM_2");
	HandleModularSkeletalMeshComponent(ModularSM_3, "ModularSM_3");
	HandleModularSkeletalMeshComponent(ModularSM_4, "ModularSM_4");
	HandleModularSkeletalMeshComponent(ModularSM_5, "ModularSM_5");
}

void AModularEquipmentItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const UItemFragment* ItemFragment = UItemFunctionLibrary::FindItemDefinitionFragment(
		ItemComponent->GetItemDef(), UItemFragment_ModularCharacterMesh::StaticClass()
	);
	if (ItemFragment)
	{
		const UItemFragment_ModularCharacterMesh* ModularCharacterMeshFragment = Cast<
			UItemFragment_ModularCharacterMesh>(ItemFragment);
		const TArray<FModularCharacterMeshInfo> ModularCharacterMeshInfos = ModularCharacterMeshFragment->
			ModularCharacterMeshInfos;

		int32 MeshIndex = -1;
		for (TObjectPtr<USkeletalMeshComponent> SMComponent : ModularSMList)
		{
			MeshIndex++;

			if (MeshIndex >= ModularCharacterMeshInfos.Num())
			{
				SMComponent->SetSkeletalMesh(nullptr);
				continue;
			}
			const FModularCharacterMeshInfo& ModularCharacterMeshInfo = ModularCharacterMeshInfos[MeshIndex];
			if (!ModularCharacterMeshInfo.bShowInItemActor)
			{
				SMComponent->SetSkeletalMesh(nullptr);
				continue;
			}
			//SetSkeletalMesh
			SMComponent.Get()->SetSkeletalMesh(ModularCharacterMeshInfo.ModularMesh);
			// AttachToComponent
			SMComponent->AttachToComponent(
				ItemSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale
			);
		}
	}
	// Offset
	const UItemFragment* CommonInfoItemFragment = UItemFunctionLibrary::FindItemDefinitionFragment(
		ItemComponent->GetItemDef(), UItemFragment_CommonInfo::StaticClass()
	);
	if (CommonInfoItemFragment)
	{
		const UItemFragment_CommonInfo* ItemFragment_CommonInfo = Cast<
			UItemFragment_CommonInfo>(CommonInfoItemFragment);

		const FGameplayTag ItemTypeTag = ItemFragment_CommonInfo->ItemTypeTag;

		FVector OffsetVector = ModularEquipmentSMOffsetMap.FindRef(ItemTypeTag);
		if (!OffsetVector.IsNearlyZero())
		{
			ItemSkeletalMeshComponent->SetRelativeLocation(OffsetVector);
		}
	}
}

void AModularEquipmentItemActor::HighlightActor()
{
	Super::HighlightActor();

	for (TObjectPtr<USkeletalMeshComponent> SMComponent : ModularSMList)
	{
		SMComponent.Get()->SetRenderCustomDepth(true);
		SMComponent.Get()->SetCustomDepthStencilValue(250);
	}
}

void AModularEquipmentItemActor::UnHighlightActor()
{
	Super::UnHighlightActor();

	for (TObjectPtr<USkeletalMeshComponent> SMComponent : ModularSMList)
	{
		SMComponent.Get()->SetRenderCustomDepth(false);
	}
}

void AModularEquipmentItemActor::HandleModularSkeletalMeshComponent(TObjectPtr<USkeletalMeshComponent>& SMComp,
                                                                    const FName CompName)
{
	SMComp = CreateDefaultSubobject<USkeletalMeshComponent>(CompName);
	SMComp->SetupAttachment(ItemSkeletalMeshComponent);

	ModularSMList.Add(SMComp);
}
