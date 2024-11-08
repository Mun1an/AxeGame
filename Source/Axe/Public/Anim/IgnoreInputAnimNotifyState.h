// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "IgnoreInputAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UIgnoreInputAnimNotifyState : public UAxeAnimNotifyStateBase
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;
};
