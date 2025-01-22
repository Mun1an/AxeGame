// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterMob.h"
#include "Interact/InteractionOption.h"
#include "Interface/InteractableInterface.h"
#include "AxeCharacterNPC.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API AAxeCharacterNPC : public AAxeCharacterMob, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AAxeCharacterNPC();

	// IInteractableInterface
	virtual TObjectPtr<UInteractableComponent> GetInteractableComponent() override;
	virtual void OnStartBePreInteracting(AAxeCharacterPlayer* InteractPlayer) override;
	virtual void OnEndBePreInteracting(AAxeCharacterPlayer* InteractPlayer) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInteractableComponent> InteractableComponent;
};
