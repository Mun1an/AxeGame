// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/TipsMessageWidgetController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/UITipsMessageUserWidget.h"

void UTipsMessageWidgetController::CreateTipsMessage(const FText& TipsMessage, float ShowTime, FName Key)
{
	UUITipsMessageUserWidget* TipsWidget = CreateWidget<UUITipsMessageUserWidget>(GetWorld(), TipsMessageWidgetClass);
	TipsWidget->SetTipsMessage(TipsMessage);
	TipsWidget->SetShowTime(ShowTime);
	TipsWidget->OnFinishCreated();
}
