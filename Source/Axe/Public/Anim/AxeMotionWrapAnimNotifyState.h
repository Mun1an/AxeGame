// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "AxeMotionWrapAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UAxeMotionWrapAnimNotifyState : public UAxeAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceAngle = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceSpeedCoefficient = 1.f;
};
