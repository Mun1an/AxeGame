// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AxeUIManagerSubsystem.generated.h"

class UStackUserWidget;
/**
 * 
 */
UCLASS()
class AXE_API UAxeUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void AddUIWidget(UStackUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
	void RemoveUIWidget(UStackUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
	UStackUserWidget* PopUIWidget();
	UFUNCTION(BlueprintCallable)
	UStackUserWidget* GetTopUIWidget() const;

protected:
	APlayerController* GetPlayerController();

	void GetWidgetInUIStackByClass(TArray<UStackUserWidget*>& OutWidgets,
	                               TSubclassOf<UStackUserWidget> WidgetClass) const;

	void SetEnterUIInputMode(bool bValue);
	
private:
	TArray<UStackUserWidget*> UIStack;

	UPROPERTY()
	APlayerController* PlayerController;
};
