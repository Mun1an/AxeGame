// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Instance/ItemDefinition.h"
#include "UseItemDefinition.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class AXE_API UUseItemDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	UUseItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	TSubclassOf<UGameplayAbility> GetUseGameplayAbility() const { return UseGameplayAbility; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> UseGameplayAbility;
};
