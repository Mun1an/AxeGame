// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/AxeItemActorBase.h"
#include "ModularEquipmentItemActor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AXE_API AModularEquipmentItemActor : public AAxeItemActorBase
{
	GENERATED_BODY()

public:
	AModularEquipmentItemActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	// HighLight
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_0;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_1;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_2;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_3;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_4;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ModularSM_5;

	TArray<TWeakObjectPtr<USkeletalMeshComponent>> ModularSMList;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FVector> ModularEquipmentSMOffsetMap;

private:
	void HandleModularSkeletalMeshComponent(TObjectPtr<USkeletalMeshComponent>& SMComp, FName CompName);
};
