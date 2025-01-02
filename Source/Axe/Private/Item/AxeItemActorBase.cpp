// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/AxeItemActorBase.h"

#include "Components/SphereComponent.h"
#include "Item/Component/ItemComponent.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment_World.h"

AAxeItemActorBase::AAxeItemActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
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
	UClass* ItemDef = ItemComponent->GetItemDef();
	if (ItemDef)
	{
		const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef);
		const UItemFragment_World* ItemFragment_World = ItemDefinition->FindFragment<UItemFragment_World>();
		if (ItemFragment_World)
		{
			FVector MeshScale = ItemFragment_World->MeshScale;

			UStaticMesh* StaticMeshWorld = ItemFragment_World->StaticMesh;
			if (StaticMeshWorld)
			{
				ItemStaticMeshComponent->SetStaticMesh(StaticMeshWorld);
				ItemStaticMeshComponent->SetRelativeScale3D(MeshScale);
			}

			USkeletalMesh* SkeletalMeshWorld = ItemFragment_World->SkeletalMesh;
			if (SkeletalMeshWorld)
			{
				ItemSkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshWorld);
				ItemStaticMeshComponent->SetRelativeScale3D(MeshScale);
			}
		}
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

void AAxeItemActorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TempRotateYaw += 0.5f;
	TempRotateYaw = FMath::Fmod(TempRotateYaw, 360.f);

	// TempLocateZ = FMath::Sin(FMath::DegreesToRadians(TempRotateYaw)) * 5.f;

	if (ItemStaticMeshComponent && ItemStaticMeshComponent->GetStaticMesh())
	{
		ItemStaticMeshComponent->SetRelativeRotation(FRotator(0, TempRotateYaw, 0));
	}
	if (ItemSkeletalMeshComponent && ItemSkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		ItemSkeletalMeshComponent->SetRelativeRotation(FRotator(0, TempRotateYaw, 0));
	}
}
