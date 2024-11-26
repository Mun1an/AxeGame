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

	virtual void GetInteractionOptions(FInteractionOption& OutOptions) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;
};
