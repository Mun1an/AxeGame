// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ModularCharacterComponent.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Character/AxeCharacterPlayer.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UModularCharacterComponent::UModularCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UModularCharacterComponent::SetModularMesh(EAxeModularCharacterSM ESM, USkeletalMesh* NewSM)

{
	const AAxeCharacterPlayer* Owner = GetAxeCharacterPlayerOwner();
	USkeletalMeshComponent* RetargetCharacterMesh = Owner->GetRetargetCharacterMesh();
	USkeletalMeshComponent* SMComponent = Owner->GetModularSkeletalMeshComponentMap().FindOrAdd(ESM).Get();
	if (SMComponent == nullptr)
	{
		return;
	}
	SMComponent->SetSkinnedAssetAndUpdate(NewSM);
	SMComponent->SetLeaderPoseComponent(RetargetCharacterMesh);
}


void UModularCharacterComponent::SetAllModularMesh()
{
	const AAxeCharacterPlayer* Owner = GetAxeCharacterPlayerOwner();
	USkeletalMeshComponent* RetargetCharacterMesh = Owner->GetRetargetCharacterMesh();

	for (const TTuple<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>>& Pair : Owner->
	     GetModularSkeletalMeshComponentMap()
	)
	{
		if (Pair.Value == nullptr)
		{
			continue;
		}
		Pair.Value.Get()->SetSkinnedAssetAndUpdate(Pair.Value.Get()->GetSkeletalMeshAsset());
	}

	for (const TTuple<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>>& Pair : Owner->
	     GetModularSkeletalMeshComponentMap()
	)
	{
		if (Pair.Value == nullptr)
		{
			continue;
		}
		Pair.Value.Get()->SetLeaderPoseComponent(RetargetCharacterMesh);
	}
}

void UModularCharacterComponent::AddRow_Test(EAxeModularCharacterSM ESM,
                                             EModularCharacterGenderType GenderType,
                                             TArray<USkeletalMesh*> MeshList)
{
	TArray<FModularCharacterSMInfo*> OutRows;
	DT_ModularCharacterSM->GetAllRows(TEXT(""), OutRows);

	for (FModularCharacterSMInfo* OutRow : OutRows)
	{
		if (OutRow->ModularMeshEnum == ESM && OutRow->GenderType == GenderType)
		{
			for (USkeletalMesh* NewSM : MeshList)
			{
				OutRow->ModularMeshList.AddUnique(NewSM);
			}
			return;
		}
	}

	FModularCharacterSMInfo RowTable = FModularCharacterSMInfo();
	RowTable.ModularMeshEnum = ESM;
	RowTable.GenderType = GenderType;
	RowTable.ModularMeshList = MeshList;

	UEnum* EnumSM = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAxeModularCharacterSM"), true);
	FString EnumSMName = EnumSM->GetNameStringByIndex(EnumSM->GetIndexByValue(static_cast<int64>(ESM)));

	UEnum* EnumGender = FindObject<UEnum>(ANY_PACKAGE, TEXT("EModularCharacterGenderType"), true);
	FString EnumGenderName = EnumGender->GetNameStringByIndex(
		EnumGender->GetIndexByValue(static_cast<int64>(GenderType)));

	FName RowName = FName(*FString::Printf(TEXT("%s_%s"), *EnumSMName, *EnumGenderName));

	DT_ModularCharacterSM->AddRow(RowName, RowTable);
}

void UModularCharacterComponent::AddModularSMToDataTable_Test()
{
	if (DT_ModularCharacterSM)
	{
		TArray<FName> RowNames = DT_ModularCharacterSM->GetRowNames();
		for (FName RowName : RowNames)
		{
			DT_ModularCharacterSM->RemoveRow(RowName);
		}
	}

	TArray<USkeletalMesh*> OutSkeletalMeshes;
	GetAllSkeletalMeshesInFolder_Test(TEXT("/Game/Assets/ModularCharacters/ModularParts"), OutSkeletalMeshes);

	for (USkeletalMesh* SMesh : OutSkeletalMeshes)
	{
		FString MeshName = SMesh->GetName();
		FString RemainMeshName = MeshName.Replace(TEXT("SK_Chr_"), TEXT(""));
		const bool bIsMeshNameFemale = RemainMeshName.Contains(TEXT("_Female_"));
		const bool bIsMeshNameMale = RemainMeshName.Contains(TEXT("_Male_"));
		const bool bIsMeshNameAll = !bIsMeshNameFemale && !bIsMeshNameMale;

		EModularCharacterGenderType GenderType = EModularCharacterGenderType::All;
		GenderType = bIsMeshNameMale ? EModularCharacterGenderType::Male : GenderType;
		GenderType = bIsMeshNameFemale ? EModularCharacterGenderType::Female : GenderType;

		RemainMeshName = RemainMeshName.Replace(TEXT("_Female"), TEXT(""));
		RemainMeshName = RemainMeshName.Replace(TEXT("_Male"), TEXT(""));
		int32 LastUnderScoreIndex;
		if (RemainMeshName.FindLastChar(TEXT('_'), LastUnderScoreIndex))
		{
			FString RightPart = RemainMeshName.Mid(LastUnderScoreIndex + 1);
			if (!RightPart.IsEmpty())
			{
				RemainMeshName.RemoveFromEnd(RightPart);
				RemainMeshName.RemoveFromEnd(TEXT("_"));
			}
		}

		FString NoUnderScoreMeshName = RemainMeshName.Replace(TEXT("_"), TEXT(""));

		UEnum* EnumSM = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAxeModularCharacterSM"), true);
		int32 NumEnums = EnumSM->NumEnums();

		EAxeModularCharacterSM EnumSMValue = EAxeModularCharacterSM::None;
		for (int32 EnumIndex = 0; EnumIndex < NumEnums; ++EnumIndex)
		{
			FString InEnumName = EnumSM->GetNameStringByIndex(EnumIndex);
			const bool bIsEnumNameAll = InEnumName.Contains(TEXT("All_"));
			const bool bIsEnumNameGender = InEnumName.Contains(TEXT("Gender_"));

			if (bIsMeshNameAll && bIsEnumNameAll)
			{
				InEnumName = InEnumName.Replace(TEXT("All_"), TEXT(""));
				FString NoUnderScoreEnumName = InEnumName.Replace(TEXT("_"), TEXT(""));
				if (NoUnderScoreEnumName == NoUnderScoreMeshName ||
					NoUnderScoreEnumName.Replace(TEXT("Attachment"), TEXT("Attach")) == NoUnderScoreMeshName.Replace(
						TEXT("Attachment"), TEXT("Attach")))
				{
					EnumSMValue = static_cast<EAxeModularCharacterSM>(EnumSM->GetValueByIndex(EnumIndex));
					break;
				}
			}
			else if ((bIsMeshNameFemale || bIsMeshNameMale) && bIsEnumNameGender)
			{
				InEnumName = InEnumName.Replace(TEXT("Gender_"), TEXT(""));
				FString NoUnderScoreEnumName = InEnumName.Replace(TEXT("_"), TEXT(""));
				if (NoUnderScoreEnumName == NoUnderScoreMeshName ||
					NoUnderScoreEnumName.Replace(TEXT("Attachment"), TEXT("Attach")) == NoUnderScoreMeshName.Replace(
						TEXT("Attachment"), TEXT("Attach")))

				{
					EnumSMValue = static_cast<EAxeModularCharacterSM>(EnumSM->GetValueByIndex(EnumIndex));
					break;
				}
			}
		}

		// UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh: %s, GenderType: %d, EnumSMValue: %s"), *MeshName,
		//        static_cast<int32>(GenderType), *EnumSM->GetNameStringByIndex(int32(EnumSMValue)));

		AddRow_Test(EnumSMValue, GenderType, {SMesh});
	}
}

void UModularCharacterComponent::GetAllSkeletalMeshesInFolder_Test(const FString& FolderPath,
                                                                   TArray<USkeletalMesh*>& OutSkeletalMeshes)
{
	// Get the Asset Registry Module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Define the filter to search for skeletal meshes in the specified folder
	FARFilter Filter;
	Filter.ClassPaths.Add(USkeletalMesh::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(*FolderPath);
	Filter.bRecursivePaths = true;

	// Get the assets
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	// Iterate through the assets and add them to the output array
	for (const FAssetData& AssetData : AssetDataList)
	{
		USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(AssetData.GetAsset());
		if (SkeletalMesh)
		{
			OutSkeletalMeshes.Add(SkeletalMesh);
		}
	}
}

// Called when the game starts
void UModularCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	ModularCharacterInit();
}

void UModularCharacterComponent::ModularCharacterInit()
{
	const AAxeCharacterPlayer* Owner = GetAxeCharacterPlayerOwner();
	USkeletalMeshComponent* RetargetCharacterMeshComp = Owner->GetRetargetCharacterMesh();

	// ModularDynamicMaterial
	check(ModularDynamicMaterial)
	ModularDynamicMaterialInstance = RetargetCharacterMeshComp->CreateDynamicMaterialInstance(
		0, ModularDynamicMaterial);
	for (TTuple<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>>&
	     Pair : Owner->GetModularSkeletalMeshComponentMap())
	{
		if (Pair.Value == nullptr)
		{
			continue;
		}
		USkeletalMeshComponent* SMComponent = Pair.Value.Get();
		SMComponent->SetMaterial(0, ModularDynamicMaterialInstance);
	}

	// MaskedMaterial
	check(ModularMaskedMaterial)
	RetargetCharacterMeshComp->SetMaterial(0, ModularMaskedMaterial);

	check(DT_ModularCharacterSM)

	UEnum* EnumSM = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAxeModularCharacterSM"), true);
	int32 NumEnums = EnumSM->NumEnums();

	// Set Gender ModularMesh
	for (int32 EnumIndex = 0; EnumIndex < NumEnums; ++EnumIndex)
	{
		const EAxeModularCharacterSM EnumSMValue = static_cast<EAxeModularCharacterSM>(EnumSM->
			GetValueByIndex(EnumIndex));

		FModularCharacterSMInfo SM_Gender_Info = FindModularSMInfoFromDataTable(EnumSMValue, CharacterGenderType);

		// Gender
		if (SM_Gender_Info.ModularMeshEnum == EAxeModularCharacterSM::None)
		{
			continue;
		}
		
		// 设置互斥的模块为nullptr
		for (const EAxeModularCharacterSM& ExclusionModularMeshEnum : SM_Gender_Info.ExclusionModularMeshEnumList)
		{
			SetModularMesh(ExclusionModularMeshEnum, nullptr);
		}

		if (SM_Gender_Info.ModularMeshList.Num() > 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, SM_Gender_Info.ModularMeshList.Num() - 1);
			SetModularMesh(EnumSMValue, SM_Gender_Info.ModularMeshList[RandomIndex]);
		}
	}

	// Set All ModularMesh
	for (int32 EnumIndex = 0; EnumIndex < NumEnums; ++EnumIndex)
	{
		const EAxeModularCharacterSM EnumSMValue = static_cast<EAxeModularCharacterSM>(EnumSM->
			GetValueByIndex(EnumIndex));

		FModularCharacterSMInfo SM_All_Info = FindModularSMInfoFromDataTable(EnumSMValue, EModularCharacterGenderType::All);

		// Gender
		if (SM_All_Info.ModularMeshEnum == EAxeModularCharacterSM::None)
		{
			continue;
		}
		
		// 设置互斥的模块为nullptr
		for (const EAxeModularCharacterSM& ExclusionModularMeshEnum : SM_All_Info.ExclusionModularMeshEnumList)
		{
			SetModularMesh(ExclusionModularMeshEnum, nullptr);
		}

		if (SM_All_Info.ModularMeshList.Num() > 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, SM_All_Info.ModularMeshList.Num() - 1);
			SetModularMesh(EnumSMValue, SM_All_Info.ModularMeshList[RandomIndex]);
		}
	}
}

AAxeCharacterPlayer* UModularCharacterComponent::GetAxeCharacterPlayerOwner()
{
	if (AxeCharacterPlayerOwner == nullptr)
	{
		AActor* Actor = GetOwner();
		AxeCharacterPlayerOwner = Cast<AAxeCharacterPlayer>(Actor);
	}
	return AxeCharacterPlayerOwner;
}

FModularCharacterSMInfo UModularCharacterComponent::FindModularSMInfoFromDataTable(EAxeModularCharacterSM ESM,
	EModularCharacterGenderType GenderType) const
{
	check(DT_ModularCharacterSM)

	TArray<FModularCharacterSMInfo*> OutRows;
	DT_ModularCharacterSM->GetAllRows(TEXT(""), OutRows);
	for (const FModularCharacterSMInfo* OutRow : OutRows)
	{
		if (OutRow->ModularMeshEnum == ESM && OutRow->GenderType == GenderType)
		{
			return *OutRow;
		}
	}
	return FModularCharacterSMInfo();
}
