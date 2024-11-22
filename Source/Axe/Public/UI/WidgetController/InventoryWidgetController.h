// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "InventoryWidgetController.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UInventoryWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()
public:
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
