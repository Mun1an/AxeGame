// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/AxeItemActorBase.h"

#include "Item/Component/ItemComponent.h"

AAxeItemActorBase::AAxeItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ItemStaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
}

void AAxeItemActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	check(ItemComponent)
	UStaticMesh* StaticMeshWorld = ItemComponent->GetStaticMeshInItemFragment_World();
	ItemStaticMeshComponent->SetStaticMesh(StaticMeshWorld);
}


void AAxeItemActorBase::GetInteractionOptions(FInteractionOption& OutOptions)
{
	OutOptions = InteractionOption;
}

TSubclassOf<UItemDefinition> AAxeItemActorBase::GetPickupableItemDef()
{
	return ItemComponent->GetItemDef();
}

int32 AAxeItemActorBase::GetPickupableItemCount()
{
	return ItemComponent->GetStackCount();
}

void AAxeItemActorBase::HighlightActor()
{
	if (ItemStaticMeshComponent)
	{
		ItemStaticMeshComponent->SetRenderCustomDepth(true);
		ItemStaticMeshComponent->SetCustomDepthStencilValue(250);
	}
}

void AAxeItemActorBase::UnHighlightActor()
{
	if (ItemStaticMeshComponent)
	{
		ItemStaticMeshComponent->SetRenderCustomDepth(false);
	}
}
