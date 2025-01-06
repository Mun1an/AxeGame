// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractionOption.h"
#include "Interface/HighLight.h"
#include "Interface/InteractableInterface.h"
#include "Interface/Pickupable.h"
#include "AxeWorldItemActorBase.generated.h"

class USphereComponent;
class UItemComponent;

UCLASS(Abstract)
class AXE_API AAxeWorldItemActorBase : public AActor, public IInteractableInterface, public IPickupable, public IHighLight

{
	GENERATED_BODY()

public:
	AAxeWorldItemActorBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	// IInteractableInterface
	virtual void GetInteractionOptions(FInteractionOption& OutOptions) override;

	// IPickupable
	virtual TSubclassOf<UItemDefinition> GetPickupableItemDef() override;
	virtual int32 GetPickupableItemCount() override;

	// HighLight
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> ItemSkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;

private:
	float TempRotateYaw = 0;
	float TempLocateZ = 0;

};
