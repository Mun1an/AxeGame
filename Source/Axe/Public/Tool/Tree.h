// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class AXE_API UTreeNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	int32 Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	TArray<UTreeNode*> TreeChildren;

	UFUNCTION(BlueprintCallable, Category = "Tree")
	FORCEINLINE void AddTreeChild(UTreeNode* Child)
	{
		TreeChildren.Add(Child);
	}
};

UCLASS(Blueprintable)
class AXE_API UTree : public UObject
{
	GENERATED_BODY()

public:
	UTree()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tree")
	UTreeNode* TreeRoot = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Tree")
	FORCEINLINE void SetRoot(UTreeNode* RootNode)
	{
		TreeRoot = RootNode;
	}
};
