// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityUIDataAsset.h"

FAxeAbilityUIInfo UAbilityUIDataAsset::FindAbilityInfoByTag(const FGameplayTag& Tag) const
{
	for (const FAxeAbilityUIInfo& AbilityInfo : AxeAbilityUIInfoList)
	{
		if (AbilityInfo.AbilityTag == Tag)
		{
			return AbilityInfo;
		}
	}
	return FAxeAbilityUIInfo();
}
