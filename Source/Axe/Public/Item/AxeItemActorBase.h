// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractionOption.h"
#include "Interface/InteractableInterface.h"
#include "AxeItemActorBase.generated.h"

class UItemComponent;

UCLASS(Abstract)
class AXE_API AAxeItemActorBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AAxeItemActorBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetInteractionOptions(FInteractionOption& OutOptions) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;
};
