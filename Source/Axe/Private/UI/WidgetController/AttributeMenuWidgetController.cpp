// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/Data/AttributeUIDataAsset.h"
#include "PlayerState/AxePlayerState.h"

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

	const AAxePlayerState* AxePS = GetAxePlayerState();
	
	AxePS->OnLevelChangedDelegate.Broadcast(AxePS->GetPlayerLevel());
	AxePS->OnXpChangedDelegate.Broadcast(AxePS->GetPlayerXp());
	AxePS->OnGoldCoinCountChangedDelegate.Broadcast(AxePS->GetGoldCoinCount());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	//
	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : GetAxeAttributeSet()->GetTagsToAttributesFuncPtrMap())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	// PlayerStateValue
	AAxePlayerState* AxePS = GetAxePlayerState();
	AxePS->OnXpChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnXpChanged);
	AxePS->OnLevelChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnPlayerLevelChanged);
	AxePS->OnGoldCoinCountChangedDelegate.AddUObject(this, &UAttributeMenuWidgetController::OnGoldCoinCountChanged);
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

void UAttributeMenuWidgetController::OnXpChanged(int32 NewValue)
{
	OnXpChangedDelegate.Broadcast(NewValue);
}

void UAttributeMenuWidgetController::OnPlayerLevelChanged(int32 NewValue)
{
	OnPlayerLevelChangedDelegate.Broadcast(NewValue);
}

void UAttributeMenuWidgetController::OnGoldCoinCountChanged(int32 NewValue)
{
	OnGoldCoinCountChangedDelegate.Broadcast(NewValue);
}
