// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MobOverlayWidgetController.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"

void UMobOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	// init
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();

	OnHealthChanged.Broadcast(LocalAxeAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LocalAxeAttributeSet->GetMaxHealth());

	OnToughnessChanged.Broadcast(LocalAxeAttributeSet->GetToughness());
	OnMaxToughnessChanged.Broadcast(LocalAxeAttributeSet->GetMaxToughness());
}

void UMobOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	// 绑定委托回调
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();

	FOnGameplayAttributeValueChange& HealthChangeDelegate = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(LocalAxeAttributeSet->GetHealthAttribute());
	HealthChangeDelegate.AddUObject(this, &UMobOverlayWidgetController::HealthChanged);
	FOnGameplayAttributeValueChange& MaxHealthChangeDelegate = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(LocalAxeAttributeSet->GetMaxHealthAttribute());
	MaxHealthChangeDelegate.AddUObject(this, &UMobOverlayWidgetController::MaxHealthChanged);

	FOnGameplayAttributeValueChange& ToughnessChangeDelegate = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(LocalAxeAttributeSet->GetToughnessAttribute());
	ToughnessChangeDelegate.AddUObject(this, &UMobOverlayWidgetController::ToughnessChanged);
	FOnGameplayAttributeValueChange& MaxToughnessChangeDelegate = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(LocalAxeAttributeSet->GetMaxToughnessAttribute());
	MaxToughnessChangeDelegate.AddUObject(this, &UMobOverlayWidgetController::MaxToughnessChanged);
}

void UMobOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(GetAxeAttributeSet()->GetHealth());
}

void UMobOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(GetAxeAttributeSet()->GetMaxHealth());
}
void UMobOverlayWidgetController::ToughnessChanged(const FOnAttributeChangeData& Data)
{
	OnToughnessChanged.Broadcast(GetAxeAttributeSet()->GetToughness());
}

void UMobOverlayWidgetController::MaxToughnessChanged(const FOnAttributeChangeData& Data)
{
	OnMaxToughnessChanged.Broadcast(GetAxeAttributeSet()->GetMaxToughness());
}
