// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "Inventory/Component/InventoryComponent.h"
#include "PlayerController/AxePlayerController.h"
#include "PlayerState/AxePlayerState.h"
#include "TipsMessage/TipsMessageSubsystem.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UOverlayWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	Super::SetWidgetControllerParams(Params);

	check(AxePlayerState)
	InventoryComponent = AxePlayerState->GetInventoryComponent();
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	// init
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();

	OnHealthChanged.Broadcast(LocalAxeAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LocalAxeAttributeSet->GetMaxHealth());
	OnStaminaChanged.Broadcast(LocalAxeAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(LocalAxeAttributeSet->GetMaxStamina());

	BroadcastAbilityInfo();
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	// 绑定委托回调

	// AxeASC
	const UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent();

	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetHealthAttribute()
	).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetMaxHealthAttribute()
	).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetStaminaAttribute()
	).AddUObject(this, &UOverlayWidgetController::StaminaChanged);
	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetMaxStaminaAttribute()
	).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);

	//
	AxeASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

	// item
	InventoryComponent->OnSendInventoryItemUIMessage.AddDynamic(
		this, &UOverlayWidgetController::SendInventoryItemUIMessage);


	// TipsMessage
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UTipsMessageSubsystem* TipsMessageSubsystem = LocalPlayer->GetGameInstance()->GetSubsystem<UTipsMessageSubsystem>();
	TipsMessageSubsystem->OnSendTipsMessageSignature.AddDynamic(this, &UOverlayWidgetController::OnSendTipsMessage);
}

void UOverlayWidgetController::BroadcastAbilityInfo()
{
	UAxeAbilitySystemComponent* AxeASC = GetAxeAbilitySystemComponent();
	if (!AxeASC)
	{
		return;
	}
	FAbilitySpecDataDelegate DataDelegate;
	DataDelegate.BindUObject(this, &UOverlayWidgetController::OnGetActivateAbilitySpec);
	AxeASC->ExecuteDelegateToGetAbilitySpec(DataDelegate);
}

void UOverlayWidgetController::OnGetActivateAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
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
		return;
	}
	AbilityUIInfo.InputTag = AxeASC->GetInputTagFromSpec(AbilitySpec);
	//
	AbilityInfoDelegate.Broadcast(AbilityUIInfo);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::SendInventoryItemUIMessage(UTexture2D* Texture, FText ItemName, int32 StackCount)
{
	OnSendInventoryItemUIMessageSignature.Broadcast(Texture, ItemName, StackCount);
}

void UOverlayWidgetController::OnSendTipsMessage(const FString& Message)
{
	OnSendTipsMessageUISignature.Broadcast(Message);
}
