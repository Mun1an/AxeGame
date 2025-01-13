// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interact/InteractionOption.h"
#include "Interface/HighLight.h"
#include "Interface/InteractableInterface.h"
#include "Interface/Pickupable.h"
#include "AxeWorldItemActor.generated.h"

class UItemInfoWidgetComponent;
class UWidgetComponent;
class ADisplayItemActor;
class USphereComponent;
class UItemComponent;

UCLASS(Blueprintable)
class AXE_API AAxeWorldItemActor : public AActor, public IInteractableInterface, public IPickupable,
                                   public IHighLight

{
	GENERATED_BODY()

public:
	AAxeWorldItemActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// IInteractableInterface
	virtual void GetInteractionOptions(FInteractionOption& OutOptions) override;

	// IPickupable
	virtual UItemInstance* GetPickupableItemInstance() override;
	virtual int32 GetPickupableItemCount() override;

	// HighLight
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	TObjectPtr<UItemInfoWidgetComponent> GetItemInfoWidgetComponent() { return ItemInfoWidgetComponent; }
	
	TObjectPtr<UItemComponent> GetItemComponent() { return ItemComponent; }
	
	UFUNCTION(BlueprintCallable)
	void InitDisplayItemActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxCollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ItemStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> ItemSkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemInfoWidgetComponent> ItemInfoWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UItemComponent> ItemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;

	UPROPERTY()
	TArray<UMeshComponent*> DisplayMeshComponents = {};
	UPROPERTY(Replicated)
	ADisplayItemActor* DisplayItemActor = nullptr;

	
	
private:
	float TempRotateYaw = 0;
	float TempLocateZ = 0;
};
