// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitTraceAbilityInterface.generated.h"

class UAbilityTask;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitTraceAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXE_API IHitTraceAbilityInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Ans_HitTrace_NotifyBegin(UAnimNotifyState* AnimNotifyState) = 0;
	UFUNCTION()
	virtual void Ans_HitTrace_NotifyEnd(UAnimNotifyState* AnimNotifyState) = 0;
	UFUNCTION()
	virtual void OnHitTrace(const FHitResult& HitResults) = 0;
};
