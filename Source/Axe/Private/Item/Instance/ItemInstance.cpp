// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/ItemInstance.h"

#include "AbilitySystemInterface.h"
#include "Engine/ActorChannel.h"
#include "Item/ItemFunctionLibrary.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/ItemFragment/ItemFragment_Shop.h"
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
	DOREPLIFETIME(UItemInstance, ItemInstanceDescription);
	DOREPLIFETIME(UItemInstance, ItemInstancePrice);
}

void UItemInstance::SetItemDef(TSubclassOf<UItemDefinition> InDef)
{
	ItemDef = InDef;
}

void UItemInstance::FinishItemInstanceCreated()
{
	InitItemInstancePrice();

	CreateItemDescription();
}

void UItemInstance::UpdateItemInstance()
{
	InitItemInstancePrice();
	CreateItemDescription();
}

const FString& UItemInstance::GetItemDescription()
{
	if (ItemInstanceDescription.Len() <= 0)
	{
		ItemInstanceDescription = CreateItemDescription();
	}
	return ItemInstanceDescription;
}

FString UItemInstance::CreateItemDescription()
{
	FString Description;

	int32 Price = GetItemInstancePrice();
	Description += FString::Printf(TEXT("价格: %d\n"), Price);
	Description += "\n";

	if (const UItemDefinition* ItemDefinition = ItemDef->GetDefaultObject<UItemDefinition>())
	{
		if (const UItemFragment_UI* ItemFragment_UI = ItemDefinition->FindFragment<UItemFragment_UI>())
		{
			Description += ItemFragment_UI->DefaultDescription.ToString();
			Description += "\n";
		}
	}
	return Description;
}

int32 UItemInstance::GetItemInstanceDefaultPrice() const
{
	const UItemDefinition* ItemDefinition = ItemDef->GetDefaultObject<UItemDefinition>();
	const UItemFragment_Shop* ItemFragment_Shop = ItemDefinition->FindFragment<UItemFragment_Shop>();
	if (ItemFragment_Shop)
	{
		return ItemFragment_Shop->DefaultPrice;
	}
	return 0;
}

void UItemInstance::InitItemInstancePrice()
{
	if (ItemInstancePrice < 0)
	{
		SetItemInstancePrice(GetItemInstanceDefaultPrice());
	}
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
