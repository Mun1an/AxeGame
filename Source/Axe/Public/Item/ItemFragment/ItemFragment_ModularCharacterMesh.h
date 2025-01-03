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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector WeaponOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FRotator WeaponRotation =  FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector WeaponScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector WeaponSecondaryOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FRotator WeaponSecondaryRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FVector WeaponSecondaryScale = FVector(1.0f, 1.0f, 1.0f);
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
