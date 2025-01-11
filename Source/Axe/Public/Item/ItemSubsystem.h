// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

class UEquipmentRarityDataAsset;
class UAxeItemSettings;
/**
 * 
 */
UCLASS()
class AXE_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UItemSubsystem();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	UEquipmentRarityDataAsset* GetEquipmentRarityDataAsset() const { return EquipmentRarityDataAsset; }

protected:
	UPROPERTY()
	TObjectPtr<UEquipmentRarityDataAsset> EquipmentRarityDataAsset;
};
