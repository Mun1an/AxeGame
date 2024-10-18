// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AxeInputDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAxeInputActionStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AXE_API UAxeInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionByInputTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;
	FGameplayTag FindAbilityTagByInputTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAxeInputActionStruct> AbilityInputActionList;
};
