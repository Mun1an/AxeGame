// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Item/Instance/ItemInstance.h"
#include "ItemInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UItemInfoUserWidget : public UAxeUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InitItemInfoWidget(UItemInstance* InItemInstance);

protected:
};
