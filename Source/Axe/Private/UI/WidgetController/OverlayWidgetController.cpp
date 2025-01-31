// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "Inventory/Component/InventoryComponent.h"
#include "PlayerState/AxePlayerState.h"
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
	OnToughnessChanged.Broadcast(LocalAxeAttributeSet->GetToughness());
	OnMaxToughnessChanged.Broadcast(LocalAxeAttributeSet->GetMaxToughness());

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
	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetToughnessAttribute()
	).AddUObject(this, &UOverlayWidgetController::ToughnessChanged);
	AxeASC->GetGameplayAttributeValueChangeDelegate(
		LocalAxeAttributeSet->GetMaxToughnessAttribute()
	).AddUObject(this, &UOverlayWidgetController::MaxToughnessChanged);

	//
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	AxeASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

	// item
	InventoryComponent->OnSendInventoryItemUIMessage.AddDynamic(
		this, &UOverlayWidgetController::SendInventoryItemUIMessage);

	// PlayerStateValue
	AAxePlayerState* AxePS = GetAxePlayerState();
	AxePS->OnXpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
	AxePS->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);

	// UseItemBar
	InventoryComponent->OnInventoryChangedDelegate.AddDynamic(
		this, &UOverlayWidgetController::OnInventoryChangedCallback);

	//
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

void UOverlayWidgetController::ToughnessChanged(const FOnAttributeChangeData& Data) const
{
	OnToughnessChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxToughnessChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxToughnessChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnXpChanged(int32 NewValue, int32 OldValue)
{
	OnXpChangedDelegate.Broadcast(NewValue);
}

void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewValue, int32 OldValue)
{
	OnPlayerLevelChangedDelegate.Broadcast(NewValue);
}

void UOverlayWidgetController::SendInventoryItemUIMessage(UTexture2D* Texture, FText ItemName, int32 StackCount)
{
	OnSendInventoryItemUIMessageSignature.Broadcast(Texture, ItemName, StackCount);
}

void UOverlayWidgetController::OnSendTipsMessage(const FString& Message)
{
	OnSendTipsMessageUISignature.Broadcast(Message);
}

void UOverlayWidgetController::OnInventoryChangedCallback(int32 SlotIndex, UItemInstance* NewItemInstance,
                                                          int32 NewCount, UItemInstance* OldItemInstance,
                                                          int32 OldCount)
{
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	FInventoryEntry& Entry = InventoryComponent->GetInventoryEntryByIndex(SlotIndex);
	const bool bHasUseItemBarTag = InventoryComponent->CheckEntryHasTag(AxeGameplayTags.Inventory_Entry_UseBar, Entry);
	if (bHasUseItemBarTag)
	{
		OnUseItemBarChangedDelegate.Broadcast(SlotIndex, NewItemInstance, NewCount, OldCount);
	}
}
