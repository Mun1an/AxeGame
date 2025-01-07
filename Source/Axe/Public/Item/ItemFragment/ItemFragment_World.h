// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_World.generated.h"

class ADisplayItemActor;

USTRUCT(BlueprintType)
struct FAxeWorldDisplayMeshInfo

{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector MeshOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator MeshRotator = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector MeshScale = FVector(1.0f, 1.0f, 1.0f);
};

/**
 * 
 */
UCLASS()
class AXE_API UItemFragment_World : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_World()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TSubclassOf<ADisplayItemActor> DisplayItemActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TArray<FAxeWorldDisplayMeshInfo> AxeWorldDisplayMeshInfos;
};
