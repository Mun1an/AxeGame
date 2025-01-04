// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AxeAnimNotifyStateBase.h"
#include "CustomNameAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UCustomNameAnimNotifyState : public UAxeAnimNotifyStateBase
{
	GENERATED_BODY()

public:
	UCustomNameAnimNotifyState(const FObjectInitializer& ObjectInitializer);
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName CustomName;
};
