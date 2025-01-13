// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ItemInfoWidgetComponent.generated.h"

class UItemInfoUserWidget;
/**
 * 
 */
UCLASS()
class AXE_API UItemInfoWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetItemInfoVisibility(bool bIsVisible);

protected:
	UFUNCTION(BlueprintCallable)
	void CreateItemInfoWidget();

	UPROPERTY()
	UItemInfoUserWidget* ItemInfoWidget = nullptr;
};
