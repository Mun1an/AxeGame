// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntryBaseComponent.h"
#include "Components/ActorComponent.h"
#include "SmithyComponent.generated.h"


struct FShopItemDataInfo;
class UShopItemDataAsset;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API USmithyComponent : public UEntryBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmithyComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
