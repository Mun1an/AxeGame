// Fill out your copyright notice in the Description page of Project Settings.


#include "TipsMessage/TipsMessageFunctionLibrary.h"

#include "UI/HUD/AxeHUD.h"
#include "UI/WidgetController/TipsMessageWidgetController.h"

void UTipsMessageFunctionLibrary::SendTipsMessage(const UObject* WorldContextObject, const FString& Message, float ShowTime, FName Key)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}
	AHUD* HUD = World->GetFirstPlayerController()->GetHUD();
	if (HUD)
	{
		AAxeHUD* AxeHUD = Cast<AAxeHUD>(HUD);
		UTipsMessageWidgetController* TipsMessageWidgetController = AxeHUD->GetTipsMessageWidgetController();
		TipsMessageWidgetController->CreateTipsMessage(FText::FromString(Message), ShowTime, Key);
	}
}
