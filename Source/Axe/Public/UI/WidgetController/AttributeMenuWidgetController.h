// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OverlayWidgetController.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeUIDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAxeAttributeUIInfo&, AttributeInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AXE_API UAttributeMenuWidgetController : public UAxeWidgetControllerBase
{
	GENERATED_BODY()

public:
	UAttributeMenuWidgetController();

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Xp")
	FOnPlayerStatChangedSignature OnXpChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="GAS|GoldCoin")
	FOnPlayerStatChangedSignature OnGoldCoinCountChangedDelegate;

protected:
	void BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute);

	void OnXpChanged(int32 NewValue);
	void OnPlayerLevelChanged(int32 NewValue);
	void OnGoldCoinCountChanged(int32 NewValue);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAttributeUIDataAsset> AttributeDataAsset;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> ShowingAttributeTags;
};
