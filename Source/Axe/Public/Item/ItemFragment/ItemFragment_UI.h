// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_UI.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UItemFragment_UI : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_UI()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FText DisplayName;
};
