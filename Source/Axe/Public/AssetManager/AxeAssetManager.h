// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AxeAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UAxeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;

	virtual void FinishInitialLoading() override;
};
