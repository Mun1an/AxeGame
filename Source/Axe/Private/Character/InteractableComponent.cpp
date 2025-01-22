// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InteractableComponent.h"

#include "Components/WidgetComponent.h"
#include "UI/Widget/InteractInfoUserWidget.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetVisibility(false);
}

FInteractionOption UInteractableComponent::GetInteractionOptions()
{
	return InteractionOption;
}

bool UInteractableComponent::InitInteractText()
{
	UUserWidget* UserWidget = InteractionWidgetComponent->GetWidget();
	if (UserWidget)
	{
		if (UInteractInfoUserWidget* InteractInfoUserWidget = Cast<UInteractInfoUserWidget>(UserWidget))
		{
			InteractInfoUserWidget->SetInteractText(InteractionOption.Text);
			return true;
		}
	}
	return false;
}

void UInteractableComponent::SetInteractTextVisibility(bool bVisible)
{
	if (bVisible && !bInitInteractText)
	{
		bInitInteractText = InitInteractText();
	}
	
	InteractionWidgetComponent->SetVisibility(bVisible);
}


void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractionWidgetComponent->AttachToComponent(
		GetOwner()->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
}
