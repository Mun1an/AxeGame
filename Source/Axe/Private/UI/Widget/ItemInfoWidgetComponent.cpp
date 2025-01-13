// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ItemInfoWidgetComponent.h"

#include "Item/AxeWorldItemActor.h"
#include "Item/Component/ItemComponent.h"
#include "UI/Widget/ItemInfoUserWidget.h"

void UItemInfoWidgetComponent::SetItemInfoVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		if (ItemInfoWidget == nullptr)
		{
			CreateItemInfoWidget();
		}
		SetVisibility(true);
	}
	else
	{
		SetVisibility(false);
	}
}

void UItemInfoWidgetComponent::CreateItemInfoWidget()
{
	AActor* Actor = GetOwner();
	AAxeWorldItemActor* AxeWorldItemActor = Cast<AAxeWorldItemActor>(Actor);
	const UItemComponent* ItemComponent = AxeWorldItemActor->GetItemComponent();
	UItemInstance* ItemInstance = ItemComponent->GetItemInstance();

	ItemInfoWidget = CreateWidget<UItemInfoUserWidget>(GetWorld(), WidgetClass);
	ItemInfoWidget->InitItemInfoWidget(ItemInstance);

	SetWidget(ItemInfoWidget);
}
