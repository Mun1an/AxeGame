// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AxeUserWidget.h"
#include "StackUserWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UStackUserWidget : public UAxeUserWidget
{
	GENERATED_BODY()

	
public:

	UStackUserWidget();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsSingle() const { return bIsSingle; }

	UFUNCTION(BlueprintCallable)
	bool GetIsSole() const { return bIsSole; }

	UFUNCTION(BlueprintCallable)
	bool GetShowMouseCurse() const { return bShowMouseCurse; }

protected:
	// 开启时会关闭其他的同类
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	bool bIsSingle = true;

	// 开启时会关闭其他的
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	bool bIsSole = true;

	// MouseCurse
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	bool bShowMouseCurse = true;
};
