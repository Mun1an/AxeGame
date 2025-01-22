// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/InteractInfoUserWidget.h"

void UInteractInfoUserWidget::SetInteractText(const FText& InteractText)
{
	OnInteractTextSet(InteractText);
}
