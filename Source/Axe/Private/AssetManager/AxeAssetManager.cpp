// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManager/AxeAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AssetManager/AxeGameData.h"
#include "Axe/AxeLogChannels.h"
#include "GameplayTag/AxeGameplayTags.h"

UAxeAssetManager::UAxeAssetManager()
{
}

UAxeAssetManager& UAxeAssetManager::Get()
{
	check(GEngine);

	if (UAxeAssetManager* Singleton = Cast<UAxeAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	UE_LOG(LogAxe, Fatal,
	       TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to AxeAssetManager!"));

	return *NewObject<UAxeAssetManager>();
}


void UAxeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//
	GetGameData();
	//
	FAxeGameplayTags::InitNativeGameplayTags();
	//
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UAxeAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

const UAxeGameData& UAxeAssetManager::GetGameData()
{
	return GetOrLoadTypedGameData<UAxeGameData>(AxeGameDataPath);
}

//
UPrimaryDataAsset* UAxeAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass,
                                                         const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath,
                                                         FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Loading GameData Object"), STAT_GameData, STATGROUP_LoadTime);
	if (!DataClassPath.IsNull())
	{
#if WITH_EDITOR
		FScopedSlowTask SlowTask(0, FText::Format(
			                         NSLOCTEXT("LyraEditor", "BeginLoadingGameDataTask", "Loading GameData {0}"),
			                         FText::FromName(DataClass->GetFName())));
		const bool bShowCancelButton = false;
		const bool bAllowInPIE = true;
		SlowTask.MakeDialog(bShowCancelButton, bAllowInPIE);
#endif
		UE_LOG(LogAxe, Log, TEXT("Loading GameData: %s ..."), *DataClassPath.ToString());
		SCOPE_LOG_TIME_IN_SECONDS(TEXT("    ... GameData loaded!"), nullptr);

		// This can be called recursively in the editor because it is called on demand from PostLoad so force a sync load for primary asset and async load the rest in that case
		if (GIsEditor)
		{
			Asset = DataClassPath.LoadSynchronous();
			LoadPrimaryAssetsWithType(PrimaryAssetType);
		}
		else
		{
			TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
			if (Handle.IsValid())
			{
				Handle->WaitUntilComplete(0.0f, false);

				// This should always work
				Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
			}
		}
	}

	if (Asset)
	{
		GameDataMap.Add(DataClass, Asset);
	}
	else
	{
		// It is not acceptable to fail to load any GameData asset. It will result in soft failures that are hard to diagnose.
		UE_LOG(LogAxe, Fatal,
		       TEXT(
			       "Failed to load GameData asset at %s. Type %s. This is not recoverable and likely means you do not have the correct data to run %s."
		       ), *DataClassPath.ToString(), *PrimaryAssetType.ToString(), FApp::GetProjectName());
	}

	return Asset;
}

UObject* UAxeAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(
				*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr,
				FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UAxeAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UAxeAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
