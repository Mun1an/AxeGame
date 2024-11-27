// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AxeWidgetControllerBase.h"
#include "AbilitySystem/Data/AbilityUIDataAsset.h"
#include "OverlayWidgetController.generated.h"

class UItemDefinition;
class UInventoryComponent;
class UAxeAbilitySystemComponent;
class UAbilityInfo;
class UAxeUserWidget;
struct FAxeAbilityUIInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSendInventoryItemUIMessageSignature, UTexture2D*, Texture,
											   FText, ItemName, int32, StackCount);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UOverlayWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params) override;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Signature delegates
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnSendInventoryItemUIMessageSignature OnSendInventoryItemUIMessageSignature;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;

	UFUNCTION()
	void SendInventoryItemUIMessage(UTexture2D* Texture, FText ItemName, int32 StackCount);
	// template <typename T>
	// T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UInventoryComponent> InventoryComponent;
};

// template <typename T>
// T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
// {
// 	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
// }
