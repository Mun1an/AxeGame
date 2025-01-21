// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "TipsMessageWidgetController.generated.h"

class UUITipsMessageUserWidget;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UTipsMessageWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateTipsMessage(const FText& TipsMessage, float ShowTime = 5.f, FName Key="");
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tips")
	TSubclassOf<UUITipsMessageUserWidget> TipsMessageWidgetClass;
};
