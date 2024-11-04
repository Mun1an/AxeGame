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

	UPROPERTY(EditDefaultsOnly)
	bool bFacingRotation = true;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;
};
