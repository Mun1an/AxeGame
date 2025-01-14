// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Instance/UseItemInstance.h"
#include "PotionItemInstance.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UPotionItemInstance : public UUseItemInstance
{
	GENERATED_BODY()

public:
	virtual bool CanUseItem() override;
	virtual bool OnUseItem() override;
	virtual bool GetIsReduceAfterUse() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PotionGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PotionGameplayEffectLevel = 1;
};
