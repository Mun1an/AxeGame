// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AxeWidgetControllerBase.h"
#include "AbilitySystem/Data/AbilityUIDataAsset.h"
#include "OverlayWidgetController.generated.h"

class UAxeAbilitySystemComponent;
class UAbilityInfo;
class UAxeUserWidget;
struct FAxeAbilityUIInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UOverlayWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Signature delegates
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void EffectTagsApplied(const FGameplayTagContainer& EffectTags) const;
protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;


	// template <typename T>
	// T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;
};

// template <typename T>
// T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
// {
// 	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
// }
