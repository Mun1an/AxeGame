// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ItemInfoWidgetComponent.h"

#include "Item/AxeWorldItemActor.h"
#include "Item/Component/ItemComponent.h"
#include "UI/Widget/ItemInfoUserWidget.h"

void UItemInfoWidgetComponent::SetItemInfoVisibility(bool bIsVisible)
{
	if (ItemInfoWidget == nullptr)
	{
		CreateItemInfoWidget();
	}
	if (bIsVisible)
	{
		UpdateItemInfoWidget();
		SetVisibility(true);
	}
	else
	{
		SetVisibility(false);
	}
}

void UItemInfoWidgetComponent::CreateItemInfoWidget()
{
	if (!WidgetClass)
	{
		return;
	}
	AActor* Actor = GetOwner();
	AAxeWorldItemActor* AxeWorldItemActor = Cast<AAxeWorldItemActor>(Actor);
	const UItemComponent* ItemComponent = AxeWorldItemActor->GetItemComponent();
	UItemInstance* ItemInstance = ItemComponent->GetItemInstance();
	if (!ItemInstance)
	{
		return;
	}
	ItemInfoWidget = CreateWidget<UItemInfoUserWidget>(GetWorld(), WidgetClass);
	ItemInfoWidget->InitItemInfoWidget(ItemInstance);

	SetWidget(ItemInfoWidget);
}

void UItemInfoWidgetComponent::UpdateItemInfoWidget()
{
	if (!ItemInfoWidget)
	{
		CreateItemInfoWidget();
		return;
	}
	AActor* Actor = GetOwner();
	AAxeWorldItemActor* AxeWorldItemActor = Cast<AAxeWorldItemActor>(Actor);
	const UItemComponent* ItemComponent = AxeWorldItemActor->GetItemComponent();
	UItemInstance* ItemInstance = ItemComponent->GetItemInstance();
	ItemInfoWidget->InitItemInfoWidget(ItemInstance);
}
