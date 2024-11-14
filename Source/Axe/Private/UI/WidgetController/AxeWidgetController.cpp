// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AxeWidgetControllerBase.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/Data/AbilityUIDataAsset.h"
#include "PlayerController/AxePlayerController.h"
#include "PlayerState/AxePlayerState.h"

void UAxeWidgetControllerBase::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAxeWidgetControllerBase::BroadcastInitialValues()
{
}

void UAxeWidgetControllerBase::BindCallbacksToDependencies()
{
}

void UAxeWidgetControllerBase::BroadcastAbilityInfo()
{
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent();
	if (!AxeASC)
	{
		return;
	}
	FAbilitySpecDataDelegate DataDelegate;
	DataDelegate.BindUObject(this, &UAxeWidgetControllerBase::OnGetActivateAbilitySpec);
	AxeASC->ExecuteDelegateToGetAbilitySpec(DataDelegate);
}

void UAxeWidgetControllerBase::OnGetActivateAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
	const UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent();
	if (!AxeASC)
	{
		return;
	}
	const FGameplayTag& AbilityTag = AxeASC->GetAbilityTagFromSpec(AbilitySpec);
	FAxeAbilityUIInfo AbilityUIInfo = AbilityUIDataAsset->FindAbilityInfoByTag(AbilityTag);
	if (!AbilityUIInfo.AbilityTag.IsValid())
	{
		UE_LOG(
			LogTemp, Warning, TEXT("%s -- AbilityUIInfo.AbilityTag is invalid"), *AbilityTag.GetTagName().ToString()
		);
		return;
	}
	AbilityUIInfo.InputTag = AxeASC->GetInputTagFromSpec(AbilitySpec);
	//
	AbilityInfoDelegate.Broadcast(AbilityUIInfo);
}

AAxePlayerController* UAxeWidgetControllerBase::GetAxePlayerController()
{
	if (AxePlayerController == nullptr)
	{
		AxePlayerController = Cast<AAxePlayerController>(PlayerController);
	}
	return AxePlayerController;
}

AAxePlayerState* UAxeWidgetControllerBase::GetAxePlayerState()
{
	if (AxePlayerState == nullptr)
	{
		AxePlayerState = Cast<AAxePlayerState>(PlayerState);
	}
	return AxePlayerState;
}

UAxeAbilitySystemComponent* UAxeWidgetControllerBase::GetAxeAbilitySystemComponent()
{
	if (AxeAbilitySystemComponent == nullptr)
	{
		AxeAbilitySystemComponent = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AxeAbilitySystemComponent;
}

UAxeAttributeSet* UAxeWidgetControllerBase::GetAxeAttributeSet()
{
	if (AxeAttributeSet == nullptr)
	{
		AxeAttributeSet = Cast<UAxeAttributeSet>(AttributeSet);
	}
	return AxeAttributeSet;
}
