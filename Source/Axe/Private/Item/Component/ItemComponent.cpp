// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "Item/ItemFragment/ItemFragment_World.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UStaticMesh* UItemComponent::GetStaticMeshInItemFragment_World()
{
	if (!ItemDef)
	{
		return nullptr;
	}
	const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef);
	const UItemFragment_World* ItemFragment_World = ItemDefinition->FindFragment<UItemFragment_World>();
	if (ItemFragment_World == nullptr)
	{
		return nullptr;
	}
	return ItemFragment_World->StaticMesh;
}

USkeletalMesh* UItemComponent::GetSkeletalMeshInItemFragment_World()
{
	if (!ItemDef)
	{
		return nullptr;
	}
	const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef);
	const UItemFragment_World* ItemFragment_World = ItemDefinition->FindFragment<UItemFragment_World>();
	if (ItemFragment_World == nullptr)
	{
		return nullptr;
	}
	return ItemFragment_World->SkeletalMesh;
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();
}
