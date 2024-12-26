// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	void BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAttributeUIDataAsset> AttributeDataAsset;
};
