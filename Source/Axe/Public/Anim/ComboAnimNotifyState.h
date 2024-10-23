// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyState.h"
#include "ComboAnimNotifyState.generated.h"

class UComboActionComponent;
class AAxePlayerController;

/**
 * 
 */
UCLASS()
class AXE_API UComboAnimNotifyState : public UAxeAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	                        const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

protected:
	AAxePlayerController* GetPlayerController(const USkeletalMeshComponent* MeshComp);
	UComboActionComponent* GetPlayerComboActionComponent(const USkeletalMeshComponent* MeshComp);
};
