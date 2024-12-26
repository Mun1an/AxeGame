// Fill out your copyright notice in the Description page of Project Settings.


#include "TipsMessage/TipsMessageFunctionLibrary.h"

#include "TipsMessage/TipsMessageSubsystem.h"

void UTipsMessageFunctionLibrary::SendTipsMessage(const UObject* WorldContextObject, const FString& Message)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}
	UTipsMessageSubsystem* TipsMessageSubsystem = World->GetGameInstance()->GetSubsystem<UTipsMessageSubsystem>();
	TipsMessageSubsystem->SendTipsMessage(Message);
}
