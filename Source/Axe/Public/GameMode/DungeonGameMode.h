// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySubsystem.h"
#include "GameMode/AxeGameMode.h"
#include "DungeonGameMode.generated.h"

class UEnemySubsystem;
/**
 * 
 */
UCLASS()
class AXE_API ADungeonGameMode : public AAxeGameMode
{
	GENERATED_BODY()

public:
	ADungeonGameMode();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void StartNewWave();
	void OnWaveEnd();
	
	UFUNCTION(BlueprintCallable)
	void SpawnWaveEnemy(int32 WaveIndex);
	UFUNCTION(BlueprintCallable)
	AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass, const FTransform& SpawnTransform);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> WaveInfoDataTable;

	UFUNCTION(BlueprintCallable)
	FWaveInfo GetWaveDataInfo(int32 WaveIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void CheckCurrentWave();
	//
	UFUNCTION(BlueprintCallable)
	void GetAllEnemySpawner(TArray<AActor*>& OutEnemySpawner);
	//
	UFUNCTION()
	void OnEnemyDead(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

private:
	// TObjectPtr<UEnemySubsystem> EnemySubsystem;

	bool bIsGameStarted = false;

	UPROPERTY()
	TArray<AActor*> SpawnedEnemyList;
};
