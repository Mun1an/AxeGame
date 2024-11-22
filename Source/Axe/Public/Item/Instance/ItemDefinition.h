// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.generated.h"

class UItemFragment;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class AXE_API UItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UItemFragment>> Fragments;
	
public:
	
	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;
};

