// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DisplayItemActor/DisplayItemActor_Modular.h"

#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"
#include "Axe/AxeLogChannels.h"
#include "Item/AxeWorldItemActor.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Component/ItemComponent.h"
#include "Item/ItemFragment/ItemFragment_ModularCharacterMesh.h"

ADisplayItemActor_Modular::ADisplayItemActor_Modular(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	ModularSMList = TArray<USkeletalMeshComponent*>();
}

void ADisplayItemActor_Modular::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ADisplayItemActor_Modular::OnAttachToParentActor(AActor* ParentActor)
{
	Super::OnAttachToParentActor(ParentActor);

	InitModularSMList();

	SetModularMeshByParentActor(ParentActor);
}

void ADisplayItemActor_Modular::SetModularMeshByParentActor(AActor* ParentActor)
{
	AAxeWorldItemActor* AxeWorldItemActor = Cast<AAxeWorldItemActor>(ParentActor);
	if (!AxeWorldItemActor)
	{
		return;
	}
	UItemComponent* ItemComponent = AxeWorldItemActor->GetItemComponent();
	if (!ItemComponent)
	{
		return;
	}
	const UItemFragment* ItemFragment = UItemFunctionLibrary::FindItemDefinitionFragment(
		ItemComponent->GetItemDef(), UItemFragment_ModularCharacterMesh::StaticClass()
	);
	if (!ItemFragment)
	{
		return;
	}
	const UItemFragment_ModularCharacterMesh* ModularCharacterMeshFragment = Cast<
		UItemFragment_ModularCharacterMesh>(ItemFragment);
	const TArray<FModularCharacterMeshInfo>& ModularCharacterMeshInfos = ModularCharacterMeshFragment->
		ModularCharacterMeshInfos;

	int32 MeshIndex = 0;

	for (const FModularCharacterMeshInfo& CharacterMeshInfo : ModularCharacterMeshInfos)
	{
		if (ModularSMList.Num() < MeshIndex)
		{
			UE_LOG(LogAxe, Warning, TEXT("ModularCharacterMeshInfos.Num() > ModularSMList.Num()"));
			break;
		}
		if (!CharacterMeshInfo.bShowInItemActor)
		{
			continue;
		}

		USkeletalMeshComponent* SMComponent = ModularSMList[MeshIndex];
		if (SMComponent)
		{
			//SetSkeletalMesh
			SMComponent->SetSkeletalMesh(CharacterMeshInfo.ModularMesh);

			MeshIndex++;
		}
	}
}

bool ADisplayItemActor_Modular::InitModularSMList_Implementation()
{
	return true;
}

void ADisplayItemActor_Modular::HighlightActor()
{
	Super::HighlightActor();
	if (ModularSMList.Num() > 0)
	{
		for (USkeletalMeshComponent* SM : ModularSMList)
		{
			UAxeBlueprintFunctionLibrary::SetHighLight(SM, true);
		}
	}
}

void ADisplayItemActor_Modular::UnHighlightActor()
{
	Super::UnHighlightActor();
	if (ModularSMList.Num() > 0)
	{
		for (USkeletalMeshComponent* SM : ModularSMList)
		{
			UAxeBlueprintFunctionLibrary::SetHighLight(SM, false);
		}
	}
}

void ADisplayItemActor_Modular::BeginPlay()
{
	Super::BeginPlay();
	InitModularSMList();
}

bool ADisplayItemActor_Modular::AddSMCompToModularSMList(USkeletalMeshComponent* SMComp)
{
	if (!IsValid(SMComp))
	{
		return false;
	}
	ModularSMList.AddUnique(SMComp);

	if (ModularSMList.Contains(nullptr))
	{
		ModularSMList.Remove(nullptr);
	}
	return true;
}
