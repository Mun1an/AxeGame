// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeUIDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAxeAttributeUIInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class AXE_API UAttributeUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UAttributeUIDataAsset();

	FAxeAttributeUIInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogWarning=false);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAxeAttributeUIInfo> AttributeInfoList;
};
