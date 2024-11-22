// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "AxeHUD.generated.h"

class UInventoryWidgetController;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAxeUserWidget;
/**
 * 
 */
UCLASS()
class AXE_API AAxeHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	FWidgetControllerParams GetWidgetControllerParams() const;

	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController();
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable)
	UInventoryWidgetController* GetInventoryWidgetController();
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& Params);
	
protected:
	virtual void BeginPlay() override;

private:
	//
	UPROPERTY()
	TObjectPtr<UAxeUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAxeUserWidget> OverlayWidgetClass;

	/**
	 * OverlayWidgetController
	 */

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	//
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;
};
