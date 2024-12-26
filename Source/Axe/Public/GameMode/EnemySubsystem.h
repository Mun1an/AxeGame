// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemySubsystem.generated.h"

USTRUCT(Blueprintable)
struct FWaveEnemyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EnemyClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 EnemyCount = 1;

	//
};

USTRUCT(Blueprintable)
struct FWaveInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FWaveEnemyInfo> WaveEnemyInfo;

	UPROPERTY(EditDefaultsOnly)
	int32 WaveTime = 60;
};


/**
 * 
 */
UCLASS()
class AXE_API UEnemySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void StartWave(int32 WaveIndex);
};
