// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeUserWidget.h"
#include "InteractInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UInteractInfoUserWidget : public UAxeUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractText(const FText& InteractText);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractTextSet(const FText& InteractText);

};
