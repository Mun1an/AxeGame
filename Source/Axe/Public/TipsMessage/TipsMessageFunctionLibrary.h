// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TipsMessageFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UTipsMessageFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="TipsMessageFunctionLibrary|SendTipsMessage")
	static void SendTipsMessage(const UObject* WorldContextObject, const FString& Message, float ShowTime = 5.f,
	                            FName Key = "");
};
