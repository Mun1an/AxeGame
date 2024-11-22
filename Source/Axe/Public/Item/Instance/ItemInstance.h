// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTag/GameplayTagStack.h"
#include "ItemInstance.generated.h"

class UItemDefinition;
/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

public:
	TSubclassOf<UItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}

	void SetItemDef(TSubclassOf<UItemDefinition> InDef);

protected:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated, BlueprintReadOnly)
	TSubclassOf<UItemDefinition> ItemDef;
};
