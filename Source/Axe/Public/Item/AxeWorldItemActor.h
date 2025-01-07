// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractionOption.h"
#include "Interface/HighLight.h"
#include "Interface/InteractableInterface.h"
#include "Interface/Pickupable.h"
#include "AxeWorldItemActor.generated.h"

class ADisplayItemActor;
class USphereComponent;
class UItemComponent;

UCLASS(Abstract)
class AXE_API AAxeWorldItemActor : public AActor, public IInteractableInterface, public IPickupable,
                                   public IHighLight

{
	GENERATED_BODY()

public:
	AAxeWorldItemActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	// IInteractableInterface
	virtual void GetInteractionOptions(FInteractionOption& OutOptions) override;

	// IPickupable
	virtual TSubclassOf<UItemDefinition> GetPickupableItemDef() override;
	virtual int32 GetPickupableItemCount() override;

	// HighLight
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	TObjectPtr<UItemComponent> GetItemComponent() { return ItemComponent; }
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	void InitDisplayItemActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> ItemSkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;

	UPROPERTY()
	TArray<UMeshComponent*> DisplayMeshComponents = {};
	UPROPERTY()
	ADisplayItemActor* DisplayItemActor = nullptr;

private:
	float TempRotateYaw = 0;
	float TempLocateZ = 0;
};
