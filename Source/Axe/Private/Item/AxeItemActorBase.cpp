// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/AxeItemActorBase.h"

#include "Components/SphereComponent.h"
#include "Item/Component/ItemComponent.h"

AAxeItemActorBase::AAxeItemActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;

	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ItemStaticMeshComponent->SetupAttachment(GetRootComponent());

	ItemSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	ItemSkeletalMeshComponent->SetupAttachment(GetRootComponent());

	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
}

void AAxeItemActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	check(ItemComponent)

	UStaticMesh* StaticMeshWorld = ItemComponent->GetStaticMeshInItemFragment_World();
	if (StaticMeshWorld)
	{
		ItemStaticMeshComponent->SetStaticMesh(StaticMeshWorld);
	}

	USkeletalMesh* SkeletalMeshWorld = ItemComponent->GetSkeletalMeshInItemFragment_World();
	if (SkeletalMeshWorld)
	{
		ItemSkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshWorld);
	}
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
	if (ItemStaticMeshComponent && ItemStaticMeshComponent->GetStaticMesh())
	{
		ItemStaticMeshComponent->SetRenderCustomDepth(true);
		ItemStaticMeshComponent->SetCustomDepthStencilValue(250);
	}
	if (ItemSkeletalMeshComponent && ItemSkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		ItemSkeletalMeshComponent->SetRenderCustomDepth(true);
		ItemSkeletalMeshComponent->SetCustomDepthStencilValue(250);
	}
}

void AAxeItemActorBase::UnHighlightActor()
{
	if (ItemStaticMeshComponent && ItemStaticMeshComponent->GetStaticMesh())
	{
		ItemStaticMeshComponent->SetRenderCustomDepth(false);
	}
	if (ItemSkeletalMeshComponent && ItemSkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		ItemSkeletalMeshComponent->SetRenderCustomDepth(false);
	}
}
