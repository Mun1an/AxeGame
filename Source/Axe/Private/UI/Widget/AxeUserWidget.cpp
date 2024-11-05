// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AxeUserWidget.h"

void UAxeUserWidget::SetWidgetController(UObject* Controller)
{
	WidgetController = Controller;
	OnWidgetControllerSet();
}
