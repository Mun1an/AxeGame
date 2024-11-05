// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	// init
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();

	OnHealthChanged.Broadcast(LocalAxeAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LocalAxeAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	// 绑定委托回调
	AAxePlayerState* LocalAxePlayerState = GetAxePlayerState();
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetHealthAttribute()
	).AddUObject(this, &UOverlayWidgetController::HealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
