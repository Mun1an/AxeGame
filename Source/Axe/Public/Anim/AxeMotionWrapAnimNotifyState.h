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
	UAxeMotionWrapAnimNotifyState();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	float GetTraceSpeedCoefficient() const { return TraceSpeedCoefficient; }
	bool IsFacingTarget() const { return bFacingTarget; }
	bool IsMoveToTarget() const { return bMoveToTarget; }
	bool IsMoveToTargetOnMoveInput() const { return bMoveToTargetOnMoveInput; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceSpeedCoefficient = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFacingTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bMoveToTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bMoveToTargetOnMoveInput = true;
};
