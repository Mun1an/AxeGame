// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "HitTraceAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UHitTraceAnimNotifyState : public UAxeAnimNotifyStateBase
{
	GENERATED_BODY()
public:
	UHitTraceAnimNotifyState(const FObjectInitializer& ObjectInitializer);
};
