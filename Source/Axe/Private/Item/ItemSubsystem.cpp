// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemSubsystem.h"

#include "Item/EquipmentRarityDataAsset.h"
#include "Item/Settings/AxeItemSettings.h"

UItemSubsystem::UItemSubsystem()
{
}

bool UItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UAxeItemSettings* AxeItemSettings = GetDefault<UAxeItemSettings>();
	UEquipmentRarityDataAsset* DataAsset = AxeItemSettings->EquipmentRarityDataAsset.LoadSynchronous();
	EquipmentRarityDataAsset = DataAsset;
}

void UItemSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
