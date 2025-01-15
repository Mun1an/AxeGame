// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "AbilitySystem/Data/AttributeUIDataAsset.h"
#include "GameplayTag/AxeGameplayTags.h"
#include "PlayerState/AxePlayerState.h"

UAttributeMenuWidgetController::UAttributeMenuWidgetController()
{
	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	ShowingAttributeTags = {
		//
		AxeGameplayTags.Attributes_Primary_Strength,
		AxeGameplayTags.Attributes_Primary_Dexterity,
		AxeGameplayTags.Attributes_Primary_Intelligence,
		//
		AxeGameplayTags.Attributes_Secondary_MaxHealth,
		AxeGameplayTags.Attributes_Secondary_MaxStamina,
		AxeGameplayTags.Attributes_Secondary_MaxToughness,
		AxeGameplayTags.Attributes_Secondary_BaseDamage,
		AxeGameplayTags.Attributes_Secondary_Armor,
		AxeGameplayTags.Attributes_Secondary_Evasive,
		AxeGameplayTags.Attributes_Secondary_PhysicalResistance,
		AxeGameplayTags.Attributes_Secondary_MagicResistance,
		AxeGameplayTags.Attributes_Secondary_CriticalHitChance,
		AxeGameplayTags.Attributes_Secondary_CriticalHitDamage,
		AxeGameplayTags.Attributes_Secondary_HealthRegeneration,
		AxeGameplayTags.Attributes_Secondary_StaminaRegeneration,
		AxeGameplayTags.Attributes_Secondary_ToughnessRegeneration,
		AxeGameplayTags.Attributes_Secondary_MovementSpeed,

	};
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	UAxeAttributeSet* LocalAxeAttributeSet = GetAxeAttributeSet();
	check(AttributeDataAsset);

	const TMap<FGameplayTag, FGameplayAttribute(*)()>& TagsToAttributesFuncPtrMap = LocalAxeAttributeSet->
		GetTagsToAttributesFuncPtrMap();

	for (const FGameplayTag& ShowingAttributeTag : ShowingAttributeTags)
	{
		const FGameplayAttribute& Attribute = TagsToAttributesFuncPtrMap.FindRef(ShowingAttributeTag)();
		if (Attribute.IsValid())
		{
			BroadcastAttributeInfo(ShowingAttributeTag, Attribute);
		}
	}

	const AAxePlayerState* AxePS = GetAxePlayerState();

	AxePS->OnLevelChangedDelegate.Broadcast(AxePS->GetPlayerLevel());
	AxePS->OnXpChangedDelegate.Broadcast(AxePS->GetPlayerXp());
	AxePS->OnGoldCoinCountChangedDelegate.Broadcast(AxePS->GetGoldCoinCount());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const TMap<FGameplayTag, FGameplayAttribute(*)()>& TagsToAttributesFuncPtrMap = GetAxeAttributeSet()->
		GetTagsToAttributesFuncPtrMap();
	//
	for (const FGameplayTag& ShowingAttributeTag : ShowingAttributeTags)
	{
		const FGameplayAttribute& Attribute = TagsToAttributesFuncPtrMap.FindRef(ShowingAttributeTag)();
		if (Attribute.IsValid())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
				[this, ShowingAttributeTag, Attribute](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(ShowingAttributeTag, Attribute);
				}
			);
		}
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
