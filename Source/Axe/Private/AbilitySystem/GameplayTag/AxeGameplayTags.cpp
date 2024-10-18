// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayTag/AxeGameplayTags.h"

#include "GameplayTagsManager.h"

void FAxeGameplayTags::InitNativeGameplayTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	for (const FName NativeGameplayTagName : NativeGameplayTagNames)
	{
		GameplayTagsManager.AddNativeGameplayTag(NativeGameplayTagName, FString(""));
	}
}
