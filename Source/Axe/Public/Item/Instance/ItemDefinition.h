// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemDefinition.generated.h"

class AAxeWorldItemActor;
class UItemFragment_CommonInfo;
class UItemFragment;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class AXE_API UItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item)
	FGameplayTag ItemTypeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item)
	TSubclassOf<UItemInstance> ItemInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Fragment, Instanced)
	TArray<TObjectPtr<UItemFragment>> Fragments;

	const UItemFragment* FindFragmentByClass(TSubclassOf<UItemFragment> FragmentClass) const;

	template <typename T>
	const T* FindFragment() const
	{
		for (UItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(T::StaticClass()))
			{
				return Cast<T>(Fragment);
			}
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	const UItemFragment_CommonInfo* GetCommonInfoFragment() const;

	UFUNCTION(BlueprintCallable)
	int32 GetItemMaxStackSize() const;
};
