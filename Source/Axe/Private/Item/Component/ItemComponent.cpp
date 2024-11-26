// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Item/AxeItemSubsystem.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
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
	UStaticMesh* ItemStaticMesh = Cast<UItemFragment_World>(Fragment)->StaticMesh;
	return ItemStaticMesh;
}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

}
