// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/AxeWorldItemActor.h"

#include "Components/SphereComponent.h"
#include "Item/Component/ItemComponent.h"
#include "Item/DisplayItemActor/DisplayItemActor.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment_World.h"

AAxeWorldItemActor::AAxeWorldItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SceneComponent->SetupAttachment(GetRootComponent());

	ItemStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ItemStaticMeshComponent->SetupAttachment(GetRootComponent());

	ItemSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	ItemSkeletalMeshComponent->SetupAttachment(GetRootComponent());

	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");
}

void AAxeWorldItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InitDisplayItemActor();
}


void AAxeWorldItemActor::GetInteractionOptions(FInteractionOption& OutOptions)
{
	OutOptions = InteractionOption;
}

TSubclassOf<UItemDefinition> AAxeWorldItemActor::GetPickupableItemDef()
{
	return ItemComponent->GetItemDef();
}

int32 AAxeWorldItemActor::GetPickupableItemCount()
{
	return ItemComponent->GetStackCount();
}

void AAxeWorldItemActor::HighlightActor()
{
	for (UMeshComponent* HighLightMeshComponent : DisplayMeshComponents)
	{
		HighLightMeshComponent->SetRenderCustomDepth(true);
		HighLightMeshComponent->SetCustomDepthStencilValue(250);
	}
	if (DisplayItemActor)
	{
		DisplayItemActor->HighlightActor();
	}
}

void AAxeWorldItemActor::UnHighlightActor()
{
	for (UMeshComponent* HighLightMeshComponent : DisplayMeshComponents)
	{
		HighLightMeshComponent->SetRenderCustomDepth(false);
	}
	if (DisplayItemActor)
	{
		DisplayItemActor->UnHighlightActor();
	}
}

void AAxeWorldItemActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TempRotateYaw += 0.5f;
	TempRotateYaw = FMath::Fmod(TempRotateYaw, 360.f);

	// TempLocateZ = FMath::Sin(FMath::DegreesToRadians(TempRotateYaw)) * 5.f;
	for (UMeshComponent* WorldShowMeshComponent : DisplayMeshComponents)
	{
		WorldShowMeshComponent->SetRelativeRotation(FRotator(0, TempRotateYaw, 0));
	}
	if (DisplayItemActor)
	{
		SceneComponent->SetRelativeRotation(FRotator(0, TempRotateYaw, 0));
	}
}

void AAxeWorldItemActor::Destroyed()
{
	Super::Destroyed();

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* AttachedActor : AttachedActors)
	{
		if (ADisplayItemActor* AttachedItemActor = Cast<ADisplayItemActor>(AttachedActor))
		{
			AttachedItemActor->Destroy();
		}
	}
}

void AAxeWorldItemActor::InitDisplayItemActor()
{
	check(ItemComponent)

	UClass* ItemDef = ItemComponent->GetItemDef();
	if (!ItemDef)
	{
		return;
	}

	const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef);
	const UItemFragment_World* ItemFragment_World = ItemDefinition->FindFragment<UItemFragment_World>();
	if (!ItemFragment_World)
	{
		return;
	}

	// AxeWorldDisplayMeshInfos
	ItemStaticMeshComponent->SetStaticMesh(nullptr);
	ItemSkeletalMeshComponent->SetSkeletalMesh(nullptr);

	const TArray<FAxeWorldDisplayMeshInfo>& MeshInfos = ItemFragment_World->AxeWorldDisplayMeshInfos;
	for (const FAxeWorldDisplayMeshInfo& DisplayMeshInfo : MeshInfos)
	{
		if (DisplayMeshInfo.StaticMesh)
		{
			ItemStaticMeshComponent->SetStaticMesh(DisplayMeshInfo.StaticMesh);
			DisplayMeshComponents.Add(ItemStaticMeshComponent);
		}
		if (DisplayMeshInfo.SkeletalMesh)
		{
			ItemSkeletalMeshComponent->SetSkeletalMesh(DisplayMeshInfo.SkeletalMesh);
			DisplayMeshComponents.Add(ItemSkeletalMeshComponent);
		}
		for (UMeshComponent* DisplayMeshComponent : DisplayMeshComponents)
		{
			DisplayMeshComponent->SetRelativeLocation(DisplayMeshInfo.MeshOffset);
			DisplayMeshComponent->SetRelativeRotation(DisplayMeshInfo.MeshRotator);
			DisplayMeshComponent->SetRelativeScale3D(DisplayMeshInfo.MeshScale);
		}
	}

	// DisplayActor
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	UClass* DisplayItemActorClass = ItemFragment_World->DisplayItemActorClass;
	if (!DisplayItemActorClass)
	{
		if (IsValid(DisplayItemActor))
		{
			DisplayItemActor->Destroy();
		}
		for (AActor* AttachedActor : AttachedActors)
		{
			if (ADisplayItemActor* AttachedItemActor = Cast<ADisplayItemActor>(AttachedActor))
			{
				AttachedItemActor->Destroy();
			}
		}
		return;
	}
	// if (IsValid(DisplayItemActor) && DisplayItemActor->GetClass() == DisplayItemActorClass && DisplayItemActor->
	// 	GetAttachParentActor() == this)
	// {
	// 	return;
	// }
	//
	if (IsValid(DisplayItemActor))
	{
		DisplayItemActor->Destroy();
	}
	for (AActor* AttachedActor : AttachedActors)
	{
		if (ADisplayItemActor* AttachedItemActor = Cast<ADisplayItemActor>(AttachedActor))
		{
			AttachedItemActor->Destroy();
		}
	}
	ADisplayItemActor* NewActor = GetWorld()->SpawnActorDeferred<ADisplayItemActor>(
		DisplayItemActorClass, FTransform::Identity, Owner);
	NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
	DisplayItemActor = NewActor;
	DisplayItemActor->AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	DisplayItemActor->OnAttachToParentActor(this);
}
