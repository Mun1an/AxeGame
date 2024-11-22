// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.generated.h"

class UItemInstance;
/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, BlueprintType)
class AXE_API UItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UItemInstance* Instance) const {}
};
