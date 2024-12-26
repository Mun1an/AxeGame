// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeUIDataAsset.h"

#include "Axe/AxeLogChannels.h"


UAttributeUIDataAsset::UAttributeUIDataAsset()
{
}

FAxeAttributeUIInfo UAttributeUIDataAsset::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, const bool bLogWarning)
{
	for (FAxeAttributeUIInfo& AttributeInfo : AttributeInfoList)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}

	if (bLogWarning)
	{
		UE_LOG(
			LogAxe, Error, TEXT("AttributeInfo for tag [%s] not found in [%s]"),
			*AttributeTag.ToString(),
			*GetNameSafe(this)
		);
	}

	return FAxeAttributeUIInfo();
}
