// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AxeUserWidget.generated.h"

class UUITipsMessageUserWidget;
/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UAxeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	UFUNCTION(BlueprintCallable)
	void SendUITipsMessage(const FText& TipsMessage, float ShowTime = 5.f);
};
