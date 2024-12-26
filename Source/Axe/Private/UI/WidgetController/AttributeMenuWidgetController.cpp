// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/Data/AttributeUIDataAsset.h"

UAttributeMenuWidgetController::UAttributeMenuWidgetController()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();
	check(AttributeDataAsset);

	for (auto& Pair : LocalAxeAttributeSet->GetTagsToAttributesFuncPtrMap())
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : GetAxeAttributeSet()->GetTagsToAttributesFuncPtrMap())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
                                                            const FGameplayAttribute& Attribute)
{
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();
	FAxeAttributeUIInfo AttrInfo = AttributeDataAsset->FindAttributeInfoForTag(Tag);
	if (AttrInfo.AttributeTag.IsValid())
	{
		AttrInfo.AttributeValue = Attribute.GetNumericValue(LocalAxeAttributeSet);
		AttributeInfoSignature.Broadcast(AttrInfo);
	}
}
