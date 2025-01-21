// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AxeUserWidget.h"

#include "UI/Widget/UITipsMessageUserWidget.h"

void UAxeUserWidget::SetWidgetController(UObject* Controller)
{
	WidgetController = Controller;
	OnWidgetControllerSet();
}

void UAxeUserWidget::SendUITipsMessage(const FText& TipsMessage, float ShowTime)
{
	
}
