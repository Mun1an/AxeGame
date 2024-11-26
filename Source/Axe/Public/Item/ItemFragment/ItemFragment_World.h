// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_World.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UItemFragment_World : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_World()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<UStaticMesh> StaticMesh;
};
