// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Ability/AxeGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "ComboDataAsset.generated.h"


USTRUCT()
struct FComboAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag ComboAbilityTag;
};

USTRUCT()
struct FComboAbilityInfoList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FComboAbilityInfo> ComboAbilityTagSequence;
};


/**
 * Tree
 */
UCLASS(Blueprintable)
class AXE_API UComboTreeNode : public UObject
{
	GENERATED_BODY()

public:
	UComboTreeNode()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	FGameplayTag InputTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	FGameplayTag AbilityTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	TArray<UComboTreeNode*> ChildrenList;

	UFUNCTION(BlueprintCallable, Category = "Tree")
	FORCEINLINE void AddTreeChild(UComboTreeNode* Child)
	{
		ChildrenList.Add(Child);
	}

	UFUNCTION(BlueprintCallable, Category = "Tree")
	UComboTreeNode* FindChildByInputTag(const FGameplayTag& ChildInputTag)
	{
		for (UComboTreeNode* Child : ChildrenList)
		{
			if (Child->InputTag.IsValid() && ChildInputTag.IsValid() && Child->InputTag.MatchesTagExact(ChildInputTag))
			{
				return Child;
			}
		}
		return nullptr;
	}

	UComboTreeNode* FindChildByAbilityTag(const FGameplayTag FindAbilityTag)
	{
		for (UComboTreeNode* Child : ChildrenList)
		{
			if (Child->AbilityTag.IsValid() && Child->AbilityTag.MatchesTagExact(FindAbilityTag))
			{
				return Child;
			}
		}
		return nullptr;
	}
};

UCLASS(Blueprintable)
class AXE_API UComboTree : public UObject
{
	GENERATED_BODY()

public:
	UComboTree()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	UComboTreeNode* Root = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Tree")
	FORCEINLINE void SetRoot(UComboTreeNode* RootNode)
	{
		Root = RootNode;
	}

	//
};

/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UComboDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FComboAbilityInfoList> ComboAbilityInfo;
};
