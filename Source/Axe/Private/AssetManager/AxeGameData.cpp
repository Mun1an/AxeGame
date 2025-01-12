// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetManager/AxeGameData.h"

#include "AssetManager/AxeAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AxeGameData)

UAxeGameData::UAxeGameData()
{
}

const UAxeGameData& UAxeGameData::Get()
{
	return UAxeAssetManager::Get().GetGameData();
}
