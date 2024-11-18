// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AxeItemActorBase.generated.h"

class UItemComponent;

UCLASS(Abstract)
class AXE_API AAxeItemActorBase : public AActor
{
	GENERATED_BODY()

public:
	AAxeItemActorBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;
};
