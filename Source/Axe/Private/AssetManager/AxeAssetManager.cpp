// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManager/AxeAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "GameplayTag/AxeGameplayTags.h"

void UAxeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAxeGameplayTags::InitNativeGameplayTags();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UAxeAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();

}
