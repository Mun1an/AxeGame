// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Instance/Sub/PotionItemInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"

bool UPotionItemInstance::CanUseItem()
{
	return true;
}

bool UPotionItemInstance::OnUseItem()
{
	//使用物品

	UAbilitySystemComponent* OwnerASC = GetOwnerAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(OwnerASC);
	AxeASC->ApplyEffectToSelfByClass(PotionGameplayEffect, PotionGameplayEffectLevel);
	return true;
}

bool UPotionItemInstance::GetIsReduceAfterUse()
{
	return true;
}
