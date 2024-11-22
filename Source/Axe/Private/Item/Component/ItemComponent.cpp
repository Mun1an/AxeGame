// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Component/ItemComponent.h"

#include "Item/AxeItemSubsystem.h"
#include "Item/Instance/ItemInstance.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

}
