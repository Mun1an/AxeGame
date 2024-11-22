// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemInstance.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UItemInstance::SetItemDef(TSubclassOf<UItemDefinition> InDef)
{
	ItemDef = InDef;
}
