// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EquipmentRarityDataAsset.h"

FEquipmentRarityInfo FEquipmentRarityInfo::Empty;

const FEquipmentRarityInfo& UEquipmentRarityDataAsset::GetEquipmentRarityInfo(EEquipmentRarity EquipmentRarity) const
{
	for (const FEquipmentRarityInfo& EquipmentRarityInfo : EquipmentRarityInfos)
	{
		if (EquipmentRarityInfo.EquipmentRarity == EquipmentRarity)
		{
			return EquipmentRarityInfo;
		}
	}
	return FEquipmentRarityInfo::Empty;
}
