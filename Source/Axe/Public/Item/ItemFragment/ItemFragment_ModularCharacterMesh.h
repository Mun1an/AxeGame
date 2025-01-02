// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ModularCharacterComponent.h"
#include "Item/ItemFragment/ItemFragment.h"
#include "ItemFragment_ModularCharacterMesh.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FModularCharacterMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* ModularMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAxeModularCharacterSM ModularMeshEnum = EAxeModularCharacterSM::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModularCharacterGenderType GenderTypeEnum = EModularCharacterGenderType::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowInItemActor = true;
};

USTRUCT(Blueprintable, BlueprintType)
struct FCharacterWeaponMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	UStaticMesh* WeaponMeshStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	UStaticMesh* WeaponSecondaryStaticMesh = nullptr;
};

/**
 * 
 */
UCLASS()
class AXE_API UItemFragment_ModularCharacterMesh : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_ModularCharacterMesh()
	{
		ModularCharacterMeshInfos = {};
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TArray<FModularCharacterMeshInfo> ModularCharacterMeshInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FCharacterWeaponMeshInfo CharacterWeaponMeshInfo;
};
