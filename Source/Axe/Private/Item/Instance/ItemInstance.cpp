// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemInstance.h"

#include "AbilitySystemInterface.h"
#include "Engine/ActorChannel.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment_UI.h"
#include "Net/UnrealNetwork.h"

UItemInstance::UItemInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, ItemDef);
	DOREPLIFETIME(UItemInstance, OwningPawn);
}

void UItemInstance::SetItemDef(TSubclassOf<UItemDefinition> InDef)
{
	ItemDef = InDef;
}

void UItemInstance::OnItemInstanceCreated()
{
}

FString UItemInstance::GetItemDescription()
{
	if (const UItemDefinition* ItemDefinition = ItemDef->GetDefaultObject<UItemDefinition>())
	{
		if (const UItemFragment_UI* ItemFragment_UI = ItemDefinition->FindFragment<UItemFragment_UI>())
		{
			return ItemFragment_UI->DefaultDescription.ToString();
		}
	}
	return FString();
}

UAbilitySystemComponent* UItemInstance::GetOwnerAbilitySystemComponent() const
{
	APawn* Pawn = GetPawn();
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn);
	if (AbilitySystemInterface)
	{
		return AbilitySystemInterface->GetAbilitySystemComponent();
	}
	return nullptr;
}
