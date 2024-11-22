// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_PickupIcon.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UItemFragment_PickupIcon : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_PickupIcon()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor PadColor;
};
