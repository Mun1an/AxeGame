// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModularCharacterComponent.generated.h"

class AAxeCharacterPlayer;

UENUM(BlueprintType)
enum class EAxeModularCharacterSM : uint8
{
	None,
	Gender_Head,
	Gender_Eyebrow,
	Gender_FacialHair,
	Gender_Torso,
	Gender_ArmUpperRight,
	Gender_ArmUpperLeft,
	Gender_ArmLowerRight,
	Gender_ArmLowerLeft,
	Gender_HandRight,
	Gender_HandLeft,
	Gender_Hips,
	Gender_LegRight,
	Gender_LegLeft,
	All_HeadCoverings_NoHair,
	All_HeadCoverings_BaseHair,
	All_HeadCoverings_NoFacialHair,
	All_Hair,
	All_HeadAttachmentHair,
	All_HeadAttachmentHelmet,
	All_ChestAttachment,
	All_BackAttachment,
	All_ShoulderAttachment_Right,
	All_ShoulderAttachment_Left,
	All_ElbowAttachment_Right,
	All_ElbowAttachment_Left,
	All_HipsAttachment,
	All_KneeAttachment_Right,
	All_KneeAttachment_Left,
	All_Extra,
};

UENUM(BlueprintType)
enum class EModularCharacterGenderType : uint8
{
	Female,
	Male,
	All
};

UENUM(BlueprintType)
enum class EModularCharacterHeadCoveringsType : uint8
{
	None,
	NoHair,
	BaseHair,
	NoFacialHair,
};

USTRUCT(Blueprintable, BlueprintType)
struct FModularCharacterSMInfo : public FTableRowBase
{
	GENERATED_BODY()
	FModularCharacterSMInfo()
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAxeModularCharacterSM ModularMeshEnum = EAxeModularCharacterSM::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EModularCharacterGenderType GenderType = EModularCharacterGenderType::All;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EModularCharacterHeadCoveringsType HeadCoveringsType = EModularCharacterHeadCoveringsType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<USkeletalMesh*> ModularMeshList = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EAxeModularCharacterSM> ExclusionModularMeshEnumList = {};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UModularCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UModularCharacterComponent();

	UFUNCTION(BlueprintCallable)
	void SetModularMesh(EAxeModularCharacterSM ESM, USkeletalMesh* NewSM);
	UFUNCTION(BlueprintCallable)
	void SetAllModularMesh();


	UFUNCTION(BlueprintCallable)
	void GetAllSkeletalMeshesInFolder_Test(const FString& FolderPath, TArray<USkeletalMesh*>& OutSkeletalMeshes);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void ModularCharacterInit();

	UFUNCTION()
	AAxeCharacterPlayer* GetAxeCharacterPlayerOwner();

	UFUNCTION()
	FModularCharacterSMInfo FindModularSMInfoFromDataTable(const EAxeModularCharacterSM ESM,
	                                                       const EModularCharacterGenderType GenderType) const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDataTable> DT_ModularCharacterSM;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UMaterial> ModularMaskedMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UMaterial> ModularDynamicMaterial;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> ModularDynamicMaterialInstance;

private:
	UPROPERTY()
	EModularCharacterGenderType CharacterGenderType = EModularCharacterGenderType::Male;

	UPROPERTY()
	AAxeCharacterPlayer* AxeCharacterPlayerOwner = nullptr;

	// TEST
	UFUNCTION(BlueprintCallable)
	void AddRow_Test(EAxeModularCharacterSM ESM, EModularCharacterGenderType GenderType,
	                 TArray<USkeletalMesh*> MeshList);

	UFUNCTION(BlueprintCallable)
	void AddModularSMToDataTable_Test();
};
