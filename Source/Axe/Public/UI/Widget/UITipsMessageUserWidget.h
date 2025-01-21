// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeUserWidget.h"
#include "Item/Instance/ItemInstance.h"
#include "UITipsMessageUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UUITipsMessageUserWidget : public UAxeUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetTipsMessage(const FText& Message) { TipsMessage = Message; }
	FORCEINLINE void SetShowTime(float Time) { ShowTime = Time; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFinishCreated();

protected:
	UPROPERTY(BlueprintReadWrite)
	FText TipsMessage;

	UPROPERTY(BlueprintReadWrite)
	float ShowTime = 5.f;
};
