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
	const UItemFragment* Fragment = ItemDefinition->FindFragmentByClass(UItemFragment_World::StaticClass());
	if (Fragment == nullptr)
	{
		return nullptr;
	}
	return Cast<UItemFragment_World>(Fragment)->StaticMesh;
}

USkeletalMesh* UItemComponent::GetSkeletalMeshInItemFragment_World()
{
	if (!ItemDef)
	{
		return nullptr;
	}
	const UItemDefinition* ItemDefinition = GetDefault<UItemDefinition>(ItemDef);
	const UItemFragment* Fragment = ItemDefinition->FindFragmentByClass(UItemFragment_World::StaticClass());
	if (Fragment == nullptr)
	{
		return nullptr;
	}
	return Cast<UItemFragment_World>(Fragment)->SkeletalMesh;
}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

}
