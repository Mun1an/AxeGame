// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/AxeGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API ALobbyGameMode : public AAxeGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString ToMapURL;

	UFUNCTION(BlueprintCallable)
	void OnGameStart();
};
