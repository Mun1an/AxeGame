// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "Enum/AxeEnum.h"
#include "LaunchCharacterNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API ULaunchCharacterNotifyState : public UAxeAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 1000;
	UPROPERTY(EditAnywhere)
	ELaunchCharacterDirection LaunchDirection = ELaunchCharacterDirection::Lc_Forward;
	UPROPERTY(EditAnywhere)
	bool bXYOverride = false;
	UPROPERTY(EditAnywhere)
	bool bZOverride = false;

};
