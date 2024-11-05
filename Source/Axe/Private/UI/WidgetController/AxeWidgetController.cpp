// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AxeWidgetController.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "PlayerController/AxePlayerController.h"
#include "PlayerState/AxePlayerState.h"

void UAxeWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAxeWidgetController::BroadcastInitialValues()
{
}

void UAxeWidgetController::BindCallbacksToDependencies()
{
}

AAxePlayerController* UAxeWidgetController::GetAxePlayerController()
{
	if (AxePlayerController == nullptr)
	{
		AxePlayerController = Cast<AAxePlayerController>(PlayerController);
	}
	return AxePlayerController;
}

AAxePlayerState* UAxeWidgetController::GetAxePlayerState()
{
	if (AxePlayerState == nullptr)
	{
		AxePlayerState = Cast<AAxePlayerState>(PlayerState);
	}
	return AxePlayerState;
}

UAxeAbilitySystemComponent* UAxeWidgetController::GetAxeAbilitySystemComponent()
{
	if (AxeAbilitySystemComponent == nullptr)
	{
		AxeAbilitySystemComponent = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AxeAbilitySystemComponent;
}

UAxeAttributeSet* UAxeWidgetController::GetAxeAttributeSet()
{
	if (AxeAttributeSet == nullptr)
	{
		AxeAttributeSet = Cast<UAxeAttributeSet>(AttributeSet);
	}
	return AxeAttributeSet;
}
