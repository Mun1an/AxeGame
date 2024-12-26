// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DungeonGameState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API ADungeonGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentWaveIndex(int32 NewWave) { CurrentWaveIndex = NewWave; }

	UFUNCTION(BlueprintCallable)
	void AddCurrentWaveIndex() { CurrentWaveIndex++; }
	

protected:
	UPROPERTY()
	int32 CurrentWaveIndex = 0;
};
