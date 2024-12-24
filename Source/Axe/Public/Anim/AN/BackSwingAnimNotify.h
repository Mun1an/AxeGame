// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AxeAnimNotifyBase.h"
#include "BackSwingAnimNotify.generated.h"


/**
 * 
 */
UCLASS()
class AXE_API UBackSwingAnimNotify : public UAxeAnimNotifyBase
{
	GENERATED_BODY()

public:
	UBackSwingAnimNotify(const FObjectInitializer& ObjectInitializer);

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                               const FAnimNotifyEventReference& EventReference) override;
};
